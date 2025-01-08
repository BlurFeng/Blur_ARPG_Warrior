// Blur Feng All Rights Reserved.


#include "GameModes/WarriorSurvivalGameMode.h"

#include "NavigationSystem.h"
#include "WarriorDebugHelper.h"
#include "WarriorFunctionLibrary.h"
#include "Characters/WarriorEnemyCharacter.h"
#include "Engine/AssetManager.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"

void AWarriorSurvivalGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	UWarriorFunctionLibrary::TryLoadSavedGameDifficulty(CurrentGameDifficulty);

	//Debug::Print(FString::Printf(TEXT("Current Difficulty: %s") , *Debug::GetEnumString(CurrentGameDifficulty)));
}

void AWarriorSurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();

	checkf(EnemyWaveSpawnerDataTable,TEXT("Forgot to assign a valid data table in survival game mode blueprint."))

	SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::WaveStarted);

	TotalWavesToSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();

	PreLoadNextWaveEnemies();
}

void AWarriorSurvivalGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//等待生成新的波次。
	if (CurrentSurvivalGameModeState == EWarriorSurvivalGameModeState::WaveStarted)
	{
		//波次开始，更新数据。
		CurrentWaveCount++;

		FWarriorEnemyWaveSpawnerTableRow* TableRow = GetCurrentWaveSpawnerTableRow();
		CurrentWaveLimitTime = TableRow->WaveLimitTime;
		WaveLimitTimer = 0.f;
		CurrentSpawnEnemyIntervalTime = TableRow->SpawnEnemyIntervalTime;
		SpawnEnemyIntervalTimer = 0.f;
		TotalSpawnedEnemiesThisWaveCounter = 0;
		CurrentWhenWaveOverGoToNext = TableRow->WhenWaveOverGoToNext;
		
		SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::WaitSpawnNewWave);
	}
	else if (CurrentSurvivalGameModeState == EWarriorSurvivalGameModeState::WaitSpawnNewWave)
	{
		TimePassedSinceStart += DeltaTime;

		if (TimePassedSinceStart >= SpawnNewWaveWaitTime)
		{
			TimePassedSinceStart = 0.f;

			//进入生成新波次。
			SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::SpawningNewWave);
		}
	}
	//生成新波次中。
	else if (CurrentSurvivalGameModeState == EWarriorSurvivalGameModeState::SpawningNewWave)
	{
		TimePassedSinceStart += DeltaTime;

		if (TimePassedSinceStart >= SpawnEnemiesDelayTime)
		{
			TimePassedSinceStart = 0.f;

			//生成敌人。
			CheckAndTrySpawnWaveEnemies();
			
			//进入波次挑战过程中。玩家战斗中。
			SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::BattleStarted);
		}
	}
	//波次开始。
	else if (CurrentSurvivalGameModeState == EWarriorSurvivalGameModeState::BattleStarted)
	{
		//进入波次挑战过程中。玩家战斗中。
		SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::InProgress);
	}
	//波次进行中。
	else if (CurrentSurvivalGameModeState == EWarriorSurvivalGameModeState::InProgress)
	{
		//超过敌人生成间隔时间，生成敌人。
		if (CurrentSpawnEnemyIntervalTime > 0.f)
		{
			SpawnEnemyIntervalTimer += DeltaTime;
			if (SpawnEnemyIntervalTimer >= CurrentSpawnEnemyIntervalTime)
			{
				// SpawnEnemyIntervalTimer 在成功“生成敌人”后重置。因为敌人死亡也会触发“生成敌人”。
				SpawnEnemyIntervalTimer = 0.f;
				CheckAndTrySpawnWaveEnemies();
			}
		}

		//超时且未清空敌人。
		if (CurrentWaveLimitTime > 0.f && !HaveNoEnemies())
		{
			WaveLimitTimer += DeltaTime;
			if (WaveLimitTimer >= CurrentWaveLimitTime)
			{
				WaveLimitTimer = 0.f;
				//生成所有剩余的敌人。
				CheckAndTrySpawnWaveEnemies(true);

				//波次结束。
				SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::WaveTimedOut);
			}
		}
	}
	//波次完成。清空敌人进入此阶段。
	else if (CurrentSurvivalGameModeState == EWarriorSurvivalGameModeState::WaveCompleted)
	{
		CheckWaveOverGoToNext(DeltaTime);
	}
	//波次结束。超过限制时间进入此阶段。
	else if (CurrentSurvivalGameModeState == EWarriorSurvivalGameModeState::WaveTimedOut)
	{
		if (CurrentWhenWaveOverGoToNext)
		{
			CheckWaveOverGoToNext(DeltaTime);
		}
		else
		{
			SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::Failed);
		}
	}
}

void AWarriorSurvivalGameMode::RegisterSpawnEnemies(const TArray<AWarriorEnemyCharacter*>& InEnemiesToRegister)
{
	for (AWarriorEnemyCharacter* SpawnedEnemy : InEnemiesToRegister)
	{
		NativeRegisterSpawnEnemy(SpawnedEnemy);
	}
}

void AWarriorSurvivalGameMode::RegisterSpawnEnemiesInActors(const TArray<AActor*>& InActorsToRegister)
{
	for (AActor* SpawnedActor : InActorsToRegister)
	{
		if (AWarriorEnemyCharacter* SpawnedEnemy = Cast<AWarriorEnemyCharacter>(SpawnedActor))
			NativeRegisterSpawnEnemy(SpawnedEnemy);
	}
}

void AWarriorSurvivalGameMode::OnSurvivalGameModeStateToFailed()
{
	SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::Failed);
}

void AWarriorSurvivalGameMode::SetCurrentSurvivalGameModeState(const EWarriorSurvivalGameModeState InState)
{
	if (CurrentSurvivalGameModeState == InState) return;
	
	CurrentSurvivalGameModeState = InState;
	OnSurvivalGameModeStateChanged.Broadcast(CurrentSurvivalGameModeState);
}

bool AWarriorSurvivalGameMode::HasFinishedAllWaves() const
{
	return CurrentWaveCount >= TotalWavesToSpawn;
}

void AWarriorSurvivalGameMode::CheckWaveOverGoToNext(const float DeltaTime)
{
	TimePassedSinceStart += DeltaTime;

	if (HasFinishedAllWaves() || TimePassedSinceStart >= WaveCompletedWaitTime)
	{
		TimePassedSinceStart = 0.f;

		//所有波次结束。
		if (HasFinishedAllWaves())
		{
			//清空敌人，胜利。
			if (HaveNoEnemies())
				SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::AllWavesDone);
			//否则失败。
			else
				SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::Failed);
		}
		//进入下一次波次，等待生成新波次。
		else
		{
			SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::WaveStarted);
			PreLoadNextWaveEnemies();
		}
	}
}

void AWarriorSurvivalGameMode::PreLoadNextWaveEnemies()
{
	if (HasFinishedAllWaves()) return;

	PreLoadedEnemyClassMap.Empty();

	//Notes：使用 AsyncLoad 异步加载，预加载资源并缓存。
	//使用 FTableRowBase 类创建 DataTable 并配置需要加载 Actor 的 TSoftClassPtr。
	//通过 UAssetManager::GetStreamableManager().RequestAsyncLoad() 方法异步加载资源，并缓存。

	for (const FWarriorEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow(true)->EnemyWaveSpawnerDefinitions)
	{
		if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull()) continue;

		UAssetManager::GetStreamableManager().RequestAsyncLoad(
			SpawnerInfo.SoftEnemyClassToSpawn.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda(
				[SpawnerInfo, this]()
				{
					if (UClass* LoadedEnemyClass = SpawnerInfo.SoftEnemyClassToSpawn.Get())
					{
						//缓存异步加载玩成的对象。
						PreLoadedEnemyClassMap.Emplace(SpawnerInfo.SoftEnemyClassToSpawn, LoadedEnemyClass);
					}
				}
			)
		);
	}
}

FWarriorEnemyWaveSpawnerTableRow* AWarriorSurvivalGameMode::GetCurrentWaveSpawnerTableRow(const bool GetNext) const
{
	const FName RowName = FName(TEXT("Wave") + FString::FromInt(CurrentWaveCount + (GetNext ? 1 : 0)));
	FWarriorEnemyWaveSpawnerTableRow* Row = EnemyWaveSpawnerDataTable->FindRow<FWarriorEnemyWaveSpawnerTableRow>(RowName, FString());
	checkf(Row, TEXT("Could not find a valid row under the name %s in the data table."), *RowName.ToString());

	return Row;
}

int32 AWarriorSurvivalGameMode::TrySpawnWaveEnemies()
{
	if (TargetPointsArray.IsEmpty())
	{
		UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), TargetPointsArray);
	}
	checkf(!TargetPointsArray.IsEmpty(), TEXT("No valid target point found in level for spawning enemies."), *GetWorld()->GetName());
	
	uint32 EnemiesSpawnedThisTime = 0;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	for (const FWarriorEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
	{
		if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull()) continue;

		const int32 NumToSpawn = FMath::RandRange(SpawnerInfo.MinPerSpawnCount, SpawnerInfo.MaxPerSpawnCount);
		UClass* LoadedEnemyClass = PreLoadedEnemyClassMap.FindChecked(SpawnerInfo.SoftEnemyClassToSpawn);

		for (int32 i = 0; i < NumToSpawn; i++)
		{
			const int32 RandomTargetPointIndex = FMath::RandRange(0, TargetPointsArray.Num() - 1);
			const FVector SpawnOrigin = TargetPointsArray[RandomTargetPointIndex]->GetActorLocation();
			const FRotator SpawnRotation = TargetPointsArray[RandomTargetPointIndex]->GetActorForwardVector().ToOrientationRotator();

			FVector RandomLocation;
			UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this, SpawnOrigin, RandomLocation, 400.f);

			RandomLocation += FVector(0.f, 0.f, 160.f);

			AWarriorEnemyCharacter* SpawnedEnemy = GetWorld()->SpawnActor<AWarriorEnemyCharacter>(LoadedEnemyClass, RandomLocation, SpawnRotation, SpawnParameters);

			if (NativeRegisterSpawnEnemy(SpawnedEnemy))
			{
				EnemiesSpawnedThisTime++;
			}

			if (!ShouldKeepSpawnEnemies()) return EnemiesSpawnedThisTime;
		}
	}

	return EnemiesSpawnedThisTime;
}

bool AWarriorSurvivalGameMode::CheckAndTrySpawnWaveEnemies(const bool SpawnAll)
{
	bool IsSpawned = false;
	
	if (SpawnAll)
	{
		while (ShouldKeepSpawnEnemies())
		{
			TrySpawnWaveEnemies();
			IsSpawned = true;
		}
	}
	else if (ShouldKeepSpawnEnemies())
	{
		TrySpawnWaveEnemies();
		IsSpawned = true;
	}

	if (IsSpawned)
	{
		//每次生成敌人后，生成敌人间隔时间重置。
		SpawnEnemyIntervalTimer = 0.f;
	}

	return IsSpawned;
}

bool AWarriorSurvivalGameMode::ShouldKeepSpawnEnemies() const
{
	return TotalSpawnedEnemiesThisWaveCounter < GetCurrentWaveSpawnerTableRow()->TotalEnemyToSpawnThisWave;
}

bool AWarriorSurvivalGameMode::NativeRegisterSpawnEnemy(AWarriorEnemyCharacter* InEnemiesToRegister)
{
	if (!InEnemiesToRegister) return false;

	CurrentSpawnedEnemiesCounter++;
	InEnemiesToRegister->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnEnemyDestroyed);
	TotalSpawnedEnemiesThisWaveCounter++;

	return true;
}

void AWarriorSurvivalGameMode::OnEnemyDestroyed(AActor* DestroyedActor)
{
	//当生成的敌人死亡被销毁时。
	
	CurrentSpawnedEnemiesCounter--;

	//Debug::Print(FString::Printf(TEXT("CurrentSpawnedEnemiesCounter: %i, TotalSpawnedEnemiesThisWaveCounter: %i"),CurrentSpawnedEnemiesCounter,TotalSpawnedEnemiesThisWaveCounter));
	
	//确认无法继续生成敌人且没有存活的敌人。
	if (!CheckAndTrySpawnWaveEnemies() && HaveNoEnemies())
	{
		//CurrentSpawnedEnemiesCounter = 0;

		//波次结束。
		SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::WaveCompleted);
	}
}

bool AWarriorSurvivalGameMode::HaveNoEnemies() const
{
	return CurrentSpawnedEnemiesCounter == 0;
}