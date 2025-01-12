// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "WarriorHeroGameplayAbility.generated.h"

class UHeroUIComponent;
class UHeroCombatComponent;
class AWarriorHeroController;
class AWarriorHeroCharacter;

//英雄技能基类。
UCLASS()
class BLUR_ARPG_WARRIOR_API UWarriorHeroGameplayAbility : public UWarriorGameplayAbility
{
	GENERATED_BODY()

public:

	virtual void OnCheckCost(const bool bAllow, const FGameplayTag AbilityTag) override;
	virtual void OnCheckCooldown(const bool bAllow, const FGameplayTag AbilityTag) override;
	
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
	
	/// 获取技能剩余冷却时间。
	/// @param InCooldownTag 冷却GETag。 
	/// @param TotalCooldownTime 总时间。
	/// @param RemainingCooldownTime 剩余时间。
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	bool GetAbilityRemainingCooldownByTag(const FGameplayTag InCooldownTag, float& TotalCooldownTime, float& RemainingCooldownTime) const;

private:
	
	TWeakObjectPtr<AWarriorHeroCharacter> CachedWarriorHeroCharacter;
	TWeakObjectPtr<AWarriorHeroController> CachedWarriorHeroController;
	
};
