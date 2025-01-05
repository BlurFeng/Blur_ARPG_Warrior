// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "WarriorHeroGameplayAbility.generated.h"

class UHeroUIComponent;
class UHeroCombatComponent;
class AWarriorHeroController;
class AWarriorHeroCharacter;
/**
 * 
 */
UCLASS()
class BLUR_ARPG_WARRIOR_API UWarriorHeroGameplayAbility : public UWarriorGameplayAbility
{
	GENERATED_BODY()

public:
	/// 获取英雄角色
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	AWarriorHeroCharacter* GetHeroCharacterFromActorInfo();

	/// 获取英雄控制器
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	AWarriorHeroController* GetHeroControllerFromActorInfo();

	/// 获取英雄战斗组件
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	UHeroCombatComponent* GetHeroCombatComponentFromActorInfo();

	/// 获取英雄UI组件。
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	UHeroUIComponent* GetHeroUIComponentFromActorInfo();

	/// 创建英雄伤害效果查询句柄。然后可以应用GE到目标上。
	/// @param EffectClass 
	/// @param InWeaponBaseDamage 
	/// @param InCurrentAttackTypeTag 
	/// @param InUsedComboCount 此值按连击阶段增幅伤害。
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	FGameplayEffectSpecHandle MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount) const;

	/// 获取技能冷却剩余时间。
	/// @param InCooldownTag 冷却GETag。 
	/// @param TotalCooldownTime 总时间。
	/// @param RemainingCooldownTime 剩余时间。
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	bool GetAbilityRemainingCooldownByTag(FGameplayTag InCooldownTag, float& TotalCooldownTime, float& RemainingCooldownTime) const;

private:

	//Notes：TWeakObjectPtr 智能指针
	//TWeakObjectPtr 是 Unreal Engine 中一种智能指针类型，专门用来弱引用（Weak Reference）UObject 类型的对象。
	//它的设计目的是提供一种安全且高效的方式来引用可能会被销毁的对象，而不会阻止该对象被垃圾回收（Garbage Collection, GC）。因为它不会增加引用计数。
	//当 TWeakObjectPtr 所指向的对象被销毁时，它会自动变为失效状态。失效的 TWeakObjectPtr 不会造成崩溃（访问前需要手动检查有效性）。
	//由于不需要管理对象的生命周期，TWeakObjectPtr 的开销更低，非常适合缓存引用但不负责对象生命周期管理的场景。
	//应用场景：在这里，GameplayAbility是频繁创建和销毁的技能，他使用AWarriorHeroCharacter但完全不想干扰到AWarriorHeroCharacter的生命周期。
	
	TWeakObjectPtr<AWarriorHeroCharacter> CachedWarriorHeroCharacter;
	TWeakObjectPtr<AWarriorHeroController> CachedWarriorHeroController;
	
};
