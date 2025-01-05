// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"
#include "HeroGameplayAbility_PickUpStones.generated.h"

class AWarriorStoneBase;

//拾取石头道具技能。
UCLASS()
class BLUR_ARPG_WARRIOR_API UHeroGameplayAbility_PickUpStones : public UWarriorHeroGameplayAbility
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface.

	//探测范围内的石头。
	UFUNCTION(BlueprintCallable)
	void CollectStones();

	//消耗石头。
	UFUNCTION(BlueprintCallable)
	void ConsumeStones();

private:
	//探测盒距离。
	UPROPERTY(EditDefaultsOnly)
	float BoxTraceDistance = 50.f;

	//探测盒尺寸。
	UPROPERTY(EditDefaultsOnly)
	FVector TraceBoxSize = FVector(100.f);

	//探测石头通道。
	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<	EObjectTypeQuery> > StoneTraceChannel;

	//绘制Debug内容。探测盒子范围。
	UPROPERTY(EditDefaultsOnly)
	bool bDrawDebugShape = false;

	//探测到的石头。
	UPROPERTY()
	TArray<AWarriorStoneBase*> CollectedStones;
};
