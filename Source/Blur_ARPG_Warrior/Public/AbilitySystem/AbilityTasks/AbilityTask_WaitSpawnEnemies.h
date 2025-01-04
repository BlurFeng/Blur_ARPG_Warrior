// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitSpawnEnemies.generated.h"

class AWarriorEnemyCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitSpawnEnemiesDelegate, const TArray<AWarriorEnemyCharacter*>&, SpawnedEnemies);

//等待生成敌人技能任务。
UCLASS()
class BLUR_ARPG_WARRIOR_API UAbilityTask_WaitSpawnEnemies : public UAbilityTask
{
	GENERATED_BODY()

public:
	/// 等待GameplayTag事件，并生成敌人。
	/// @param OwningAbility 
	/// @param EventTag 等待事件Tag。
	/// @param SoftEnemyClassToSpawn 生成敌人类型。 
	/// @param NumToSpawn 生成数量。
	/// @param SpawnOrigin 生成原始外置。
	/// @param RandomSpawnRadius 随机生成位置范围。
	/// @param SpawnRotation 生成角度。
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Warrior|AbilityTasks", meta = (DisplayName = "Wait Gameplay Event And Spawn Enemies", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", NumToSpawn = "1", RandomSpawnRadius = "200"))
	static UAbilityTask_WaitSpawnEnemies* WaitSpawnEnemies(
		UGameplayAbility* OwningAbility,
		FGameplayTag EventTag,
		TSoftClassPtr<AWarriorEnemyCharacter> SoftEnemyClassToSpawn,
		int32 NumToSpawn,
		const FVector& SpawnOrigin,
		float RandomSpawnRadius);

	UPROPERTY(BlueprintAssignable)
	FWaitSpawnEnemiesDelegate OnSpawnFinished;

	UPROPERTY(BlueprintAssignable)
	FWaitSpawnEnemiesDelegate DidNotSpawn;

	//~ Begin UGameplayTask Interface
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
	//~ End UGameplayTask Interface

private:
	FGameplayTag CachedEventTag;
	TSoftClassPtr<AWarriorEnemyCharacter> CachedSoftEnemyClassToSpawn;
	int32 CachedNumToSpawn;
	FVector CachedSpawnOrigin;
	float CachedRandomSpawnRadius;
	FDelegateHandle DelegateHandle;

	void OnGameplayEventReceived(const FGameplayEventData* InPayload);
	void OnEnemyClassLoaded();
};
