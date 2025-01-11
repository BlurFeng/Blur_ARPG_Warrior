// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "WarriorTypes/WarriorEnumTypes.h"

#include "WarriorGameplayAbility.generated.h"

class UPawnUIComponent;
class AWarriorBaseCharacter;
class IPawnUIInterface;
class UWarriorAbilitySystemComponent;
class UPawnCombatComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCheckCostOrCooldownDelegate, const bool, bAllow, const FGameplayTag, AbilityTag);

//技能激活策略
UENUM(BlueprintType)
enum class EWarriorAbilityActivationPolicy : uint8
{
	//触发形式，通过输入或游戏事件触发的技能。
	OnTriggered,

	//给定形式，立即触发一次性的效果，并立即结束。
	OnGiven,
};

//GA基类。
UCLASS()
class BLUR_ARPG_WARRIOR_API UWarriorGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UWarriorGameplayAbility();

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual bool CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	//~ End UGameplayAbility Interface

	UFUNCTION()
	virtual void OnCheckCost(const bool bAllow, const FGameplayTag AbilityTag);
	UFUNCTION()
	virtual void OnCheckCooldown(const bool bAllow, const FGameplayTag AbilityTag);
	
public:
	///当尝试触发技能，确认Cost是否足够时。
	UPROPERTY(BlueprintAssignable, Category = "Warrior|Ability", meta = (DisplayName = "On Check Cost"))
	FOnCheckCostOrCooldownDelegate OnCheckCostDelegate;

	///当尝试触发技能，确认Cooldown是否结束。
	UPROPERTY(BlueprintAssignable, Category = "Warrior|Ability", meta = (DisplayName = "On Check Cool down"))
	FOnCheckCostOrCooldownDelegate OnCheckCooldownDelegate;

	//条件确认方法。允许蓝图覆盖的方法。
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Warrior|Ability")
	bool CheckConditionOnToggleableCancelAbility();

	// 条件确认方法。C++中实现的方法。
	virtual bool CheckConditionOnToggleableCancelAbility_Implementation();

protected:

	//技能激活策略。根据策略不同会以不同的形式运行这个技能。
	UPROPERTY(EditDefaultsOnly, Category = "Warrior|Ability")
	EWarriorAbilityActivationPolicy AbilityActivationPolicy = EWarriorAbilityActivationPolicy::OnTriggered;

	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	AWarriorBaseCharacter* GetCharacterFromActorInfo();
	
	/// 获取角色战斗组件。
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

	/// 获取角色UI组件。
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	UPawnUIComponent* GetPawnUIComponentFromActorInfo();

	/// 获取技能组件
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	UWarriorAbilitySystemComponent* GetWarriorAbilitySystemComponentFromActorInfo() const;

	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle) const;

	/// 应用一个GE到目标Actor。
	/// @param TargetActor 
	/// @param InSpecHandle 
	/// @param OutSuccessType 
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Warrior|Ability", meta = (DisplayName = "Apply Gameplay Effect Spec Handle To Target Actor", ExpandEnumAsExecs = "OutSuccessType"))
	FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EWarriorSuccessType& OutSuccessType);

	/// 应用一个GE到FHitResult碰撞探测目标数组。
	/// 可以使用 BoxTrace 等探测方式来获得目标FHitResult。
	/// @param InSpecHandle GE说明句柄。
	/// @param InHitResults Trace获得的Hit目标。
	UFUNCTION(BlueprintCallable, Category = "Warrior|Ability")
	void ApplyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle, const TArray<FHitResult>& InHitResults);

	/// 创建GE查询句柄，用于施加GE。
	/// @param EffectClass 
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Warrior|Ability")
	FGameplayEffectSpecHandle MakeSpecHandle(const TSubclassOf<UGameplayEffect> EffectClass) const;

	/// 创建GE查询句柄，用于施加GE。
	/// @param EffectClass
	/// @param GameplayTag 查询用Tag。
	/// @param Magnitude 传递数值。
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Warrior|Ability")
	FGameplayEffectSpecHandle MakeSpecHandleSetByCallerMagnitude(const TSubclassOf<UGameplayEffect> EffectClass, const FGameplayTag GameplayTag, const float Magnitude) const;

	/// 创建伤害效果查询句柄。然后可以应用GE到目标上。
	/// @param EffectClass 
	/// @param InBaseDamage 基础伤害。
	/// @param InBaseDamageMultiplyCoefficient 基础伤害乘系数。
	/// @param DamageIncreaseCount 增伤计数。增伤幅度为 1 + DamageIncreaseCount * DamageIncreaseCoefficient。
	/// @param DamageIncreaseCoefficient 增伤系数。增伤幅度为 1 + DamageIncreaseCount * DamageIncreaseCoefficient。
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability", meta = (DisplayName = "Make Damage Effect Spec Handle",InBaseDamage = "10", InBaseDamageMultiplyCoefficient = "1", DamageIncreaseCount = "0", DamageIncreaseCoefficient = "0.15"))
	FGameplayEffectSpecHandle MakeDamageEffectSpecHandle(
		const TSubclassOf<UGameplayEffect> EffectClass, const float InBaseDamage, const float InBaseDamageMultiplyCoefficient, const int32 DamageIncreaseCount, const float DamageIncreaseCoefficient) const;

	/// 创建伤害效果查询句柄。然后可以应用GE到目标上。
	/// @param EffectClass 
	/// @param InBaseDamageScalableFloat 基础伤害可变值，根据AbilityLevel变化。
	/// @param InBaseDamageMultiplyCoefficient 基础伤害乘系数。
	/// @param DamageIncreaseCount 增伤计数。增伤幅度为 1 + DamageIncreaseCount * DamageIncreaseCoefficient。
	/// @param DamageIncreaseCoefficient 增伤系数。增伤幅度为 1 + DamageIncreaseCount * DamageIncreaseCoefficient。
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability", meta = (DisplayName = "Make Damage Effect Spec Handle", InBaseDamageMultiplyCoefficient = "1", DamageIncreaseCount = "0", DamageIncreaseCoefficient = "0.15"))
	FGameplayEffectSpecHandle MakeDamageEffectSpecHandleByScalableFloat(
		const TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InBaseDamageScalableFloat, const float InBaseDamageMultiplyCoefficient, const int32 DamageIncreaseCount, const float DamageIncreaseCoefficient) const;
	
	/// 获取可变float值根据自身AbilityLevel。
	/// @param InScalableFloat 
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability", meta = (CompactNodeTitle = "Get Value At Level"))
	float GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat) const;

private:
	TWeakObjectPtr<AWarriorBaseCharacter> CachedWarriorBaseCharacter;
};
