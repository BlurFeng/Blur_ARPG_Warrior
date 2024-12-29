// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "WarriorGameplayAbility.generated.h"

class UPawnCombatComponent;
//技能激活策略
UENUM(BlueprintType)
enum class EWarriorAbilityActivationPolicy : uint8
{
	//触发形式，通过输入或游戏事件触发的技能。
	OnTriggered,

	//给定形式，立即触发一次性的效果，并立即结束。
	OnGiven,
};

/**
 * 
 */
UCLASS()
class BLUR_ARPG_WARRIOR_API UWarriorGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

	//技能激活策略。根据策略不同会以不同的形式运行这个技能。
	UPROPERTY(EditDefaultsOnly, Category = "Warrior|Ability")
	EWarriorAbilityActivationPolicy AbilityActivationPolicy = EWarriorAbilityActivationPolicy::OnTriggered;

	/// 获取人物战斗组件
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;
};
