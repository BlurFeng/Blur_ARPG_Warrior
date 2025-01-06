// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/WarriorBaseGameMode.h"
#include "WarriorSurvivalGameMode.generated.h"

class AWarriorEnemyCharacter;

//生存游戏模式状态。
UENUM(BlueprintType)
enum class EWarriorSurvivalGameModeState : uint8
{
	None = 0,

	//波次开始
	WaveStarted,
	//等待生成新的波次。
	WaitSpawnNewWave,
	//生成新的波次中。
	SpawningNewWave,
	//波次战斗开始，此状态只会停留一帧，然后切换到InProgress。
	BattleStarted,
	//进行中。玩家正在战斗。
	InProgress,
	//波次完成。
	WaveCompleted,
	//所有波次完成。
	AllWavesDone,
	//玩家死亡。
	Failed
};

//敌人波次生成器信息。
USTRUCT(BlueprintType)
struct FWarriorEnemyWaveSpawnerInfo
{
	GENERATED_BODY()

	//生成敌人类型。
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AWarriorEnemyCharacter> SoftEnemyClassToSpawn;

	//每次生成敌人的随机数最小值。
	UPROPERTY(EditAnywhere)
	int32 MinPerSpawnCount = 1;

	//每次生成敌人的随机数最大值。
	UPROPERTY(EditAnywhere)
	int32 MaxPerSpawnCount = 3;
};

//敌人波次生成器数据表数据。
USTRUCT(BlueprintType)
struct FWarriorEnemyWaveSpawnerTableRow : public FTableRowBase
{
	GENERATED_BODY()

	//敌人生成器定义数据组。
	UPROPERTY(EditAnywhere)
	TArray<FWarriorEnemyWaveSpawnerInfo> EnemyWaveSpawnerDefinitions;

	//这一波生成敌人总数。
	UPROPERTY(EditAnywhere)
	int32 TotalEnemyToSpawnThisWave = 1;

	//“生成敌人”间隔时间。正常情况下，在有敌人死亡时，触发一次“生成敌人”。但如果达到间隔时间，直接触发一次“生成敌人”。
	//当任意情况下触发“生成敌人”后，重新计时。
	UPROPERTY(EditAnywhere)
	float SpawnEnemyIntervalTime = 20.f;

	//本波次的限制时间，时间结束时直接进入下一波次。
	//如果达到波次限制时间，但还有剩余的需要生成的敌人，会直接生成全部需要数量的敌人。
	UPROPERTY(EditAnywhere)
	float WaveLimitTime = 50.f;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSurvivalGameModeStateChanged, EWarriorSurvivalGameModeState, CurrentState);

//生存游戏模式。
UCLASS()
class BLUR_ARPG_WARRIOR_API AWarriorSurvivalGameMode : public AWarriorBaseGameMode
{
	GENERATED_BODY()

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	//设置当前游戏状态。
	void SetCurrentSurvivalGameModeState(const EWarriorSurvivalGameModeState InState);

	//是否结束了所有波次。
	UFUNCTION(BlueprintCallable, Category = "WaveDefinition")
	bool HasFinishedAllWaves() const;

	//预加载下一波次敌人。
	void PreLoadNextWaveEnemies();

	//获取当前波次生成器TableRow数据。
	FWarriorEnemyWaveSpawnerTableRow* GetCurrentWaveSpawnerTableRow(const bool GetNext = false) const;
	
	//生产当前波次敌人。
	int32 TrySpawnWaveEnemies();
	bool CheckAndTrySpawnWaveEnemies(const bool SpawnAll = false);

	bool ShouldKeepSpawnEnemies() const;

	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor);
	
	
	//当前生存模式状态。
	UPROPERTY()
	EWarriorSurvivalGameModeState CurrentSurvivalGameModeState;

	//当游戏模式状态改变时委托。
	UPROPERTY(BlueprintAssignable)
	FOnSurvivalGameModeStateChanged OnSurvivalGameModeStateChanged;
	
	//敌人波次生成器数据表。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	UDataTable* EnemyWaveSpawnerDataTable;

	//总生成波次。
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	int32 TotalWavesToSpawn;

	//当前波次计数。
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	int32 CurrentWaveCount = 0;
	
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float SpawnEnemiesDelayTime = 2.f;

	//WaveCompleted 状态逻辑执行前的等待时间。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float WaveCompletedWaitTime = 5.f;

	//预加载敌人缓存Map。
	UPROPERTY()
	TMap<TSoftClassPtr<AWarriorEnemyCharacter>, UClass*> PreLoadedEnemyClassMap;

	//当前波次生成敌人时间间隔。
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float CurrentSpawnEnemyIntervalTime;
	UPROPERTY()
	float SpawnEnemyIntervalTimer;

	//当前波次限制时间。
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float CurrentWaveLimitTime;
	UPROPERTY()
	float WaveLimitTimer;
public:
	
	/// 注册生成的敌人到GameMode。
	/// 不是游戏模式生成的敌人被创建时（比如召唤技能），应当通过此方法注册到游戏模式。
	/// @param InEnemiesToRegister 
	UFUNCTION(BlueprintCallable)
	void RegisterSpawnEnemies(const TArray<AWarriorEnemyCharacter*>& InEnemiesToRegister);

	//生存游戏模式失败。玩家真正死亡时调用。
	UFUNCTION(BlueprintCallable)
	void OnSurvivalGameModeStateToFailed();
};
