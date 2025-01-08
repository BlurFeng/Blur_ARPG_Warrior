// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitSpawnActors.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitSpawnActorsDelegate, const TArray<AActor*>&, SpawnedActors);

class AActor;

//等待生成Actor技能任务。
UCLASS()
class BLUR_ARPG_WARRIOR_API UAbilityTask_WaitSpawnActors : public UAbilityTask
{
	GENERATED_BODY()

public:
	/// 等待GameplayTag事件，并生成敌人。
	/// @param OwningAbility 
	/// @param EventTag 等待事件Tag。
	/// @param SoftActorClassToSpawn 生成Actor类型。 
	/// @param NumToSpawn 生成数量。
	/// @param SpawnOrigin 生成原始外置。
	/// @param RandomSpawnRadius 随机生成位置范围。
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Warrior|AbilityTasks", meta = (DisplayName = "Wait Gameplay Event And Spawn Actors", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", NumToSpawn = "1", RandomSpawnRadius = "200"))
	static UAbilityTask_WaitSpawnActors* WaitSpawnActors(
		UGameplayAbility* OwningAbility,
		FGameplayTag EventTag,
		TSoftClassPtr<AActor> SoftActorClassToSpawn,
		int32 NumToSpawn,
		const FVector& SpawnOrigin,
		float RandomSpawnRadius);

	UPROPERTY(BlueprintAssignable)
	FWaitSpawnActorsDelegate OnSpawnFinished;

	UPROPERTY(BlueprintAssignable)
	FWaitSpawnActorsDelegate DidNotSpawn;

	//~ Begin UGameplayTask Interface
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
	//~ End UGameplayTask Interface

private:
	FGameplayTag CachedEventTag;
	TSoftClassPtr<AActor> CachedSoftActorClassToSpawn;
	int32 CachedNumToSpawn;
	FVector CachedSpawnOrigin;
	float CachedRandomSpawnRadius;
	FDelegateHandle DelegateHandle;

	void OnGameplayEventReceived(const FGameplayEventData* InPayload);
	void OnActorClassLoaded();
};
