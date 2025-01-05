// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "GameModes/WarriorBaseGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "WarriorSurvivalGameMode.generated.h"

class AWarriorEnemyCharacter;

//生存游戏模式状态。
UENUM(BlueprintType)
enum class EWarriorSurvivalGameModeState : uint8
{
	None = 0,
	
	//等待生成新的波次。
	WaitSpawnNewWave,
	//生成新的波次中。
	SpawningNewWave,
	//进行中。玩家正在战斗。
	InProgress,
	//波次完成。
	WaveCompleted,
	//所有波次完成。
	AllWavesDone,
	//玩家死亡。
	PlayerDied
};

//敌人波次生成器信息。
USTRUCT(BlueprintType)
struct FWarriorEnemyWaveSpawnerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AWarriorEnemyCharacter> SoftEnemyClassToSpawn;

	UPROPERTY(EditAnywhere)
	int32 MinPerSpawnCount = 1;

	UPROPERTY(EditAnywhere)
	int32 MaxPerSpawnCount = 3;
};

//敌人波次生成器数据表数据。
USTRUCT(BlueprintType)
struct FWarriorEnemyWaveSpawnerTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FWarriorEnemyWaveSpawnerInfo> EnemyWaveSpawnerDefinitions;

	//这一波生成敌人总数。
	UPROPERTY(EditAnywhere)
	int32 TotalEnemyToSpawnThisWave = 1;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSurvivalGameModeStateChanged, EWarriorSurvivalGameModeState, CurrentState);

//生存游戏模式。
UCLASS()
class BLUR_ARPG_WARRIOR_API AWarriorSurvivalGameMode : public AWarriorBaseGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	//设置当前游戏状态。
	void SetCurrentSurvivalGameModeState(const EWarriorSurvivalGameModeState InState);

	//是否结束了所有波次。
	bool HasFinishedAllWaves() const;

	//预加载下一波次敌人。
	void PreLoadNextWaveEnemies();

	//获取当前波次生成器TableRow数据。
	FWarriorEnemyWaveSpawnerTableRow* GetCurrentWaveSpawnerTableRow() const;
	
	//生产当前波次敌人。
	int32 TrySpawnWaveEnemies();

	bool ShouldKeepSpawnEnemies() const;

	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor);
	
	
	//当前生存模式状态。
	UPROPERTY()
	EWarriorSurvivalGameModeState CurrentSurvivalGameModeState;

	//当游戏模式状态改变时委托。
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSurvivalGameModeStateChanged OnSurvivalGameModeStateChanged;

	//敌人波次生成器数据表。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	UDataTable* EnemyWaveSpawnerDataTable;

	//总生成波次。
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	int32 TotalWavesToSpawn;

	//当前波次计数。
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	int32 CurrentWaveCount = 1;

	//当前生成敌人计数器。
	UPROPERTY()
	int32 CurrentSpawnedEnemiesCounter = 0;

	//这一波次总生成敌人计数器。
	UPROPERTY()
	int32 TotalSpawnedEnemiesThisWaveCounter = 0;

	//目标点数组。
	UPROPERTY()
	TArray<AActor*> TargetPointsArray;

	//等待计时器。
	UPROPERTY()
	float TimePassedSinceStart = 0.f;

	//WaitSpawnNewWave 状态的等待时间。之后进入 SpawningNewWave 状态。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float SpawnNewWaveWaitTime = 5.f;

	//SpawningNewWave 状态，开始生成逻辑前的一段延迟时间。生成结束后进入 InProgress 状态。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float SpawnEnemiesDelayTime = 2.f;

	//WaveCompleted 状态逻辑执行前的等待时间。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float WaveCompletedWaitTime = 5.f;

	//预加载敌人缓存Map。
	UPROPERTY()
	TMap<TSoftClassPtr<AWarriorEnemyCharacter>, UClass*> PreLoadedEnemyClassMap;
};
