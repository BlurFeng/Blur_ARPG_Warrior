// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "HeroCombatComponent.generated.h"

class AWarriorHeroWeapon;

//英雄战斗组件基类
UCLASS()
class BLUR_ARPG_WARRIOR_API UHeroCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
	/// 获取英雄武器，根据Tag。
	/// @param InWeaponTag 武器Tag。
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWarriorHeroWeapon* GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const;

	/// 获取英雄当前装备的武器
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWarriorHeroWeapon* GetHeroCurrentEquippedWeapon() const;

	/// 获取英雄当前装备武器的伤害。根据Level。
	/// @param InLevel 武器伤害Level。武器基础伤害实际上配置了曲线，根据输入Level获取相应的基础伤害。
	/// @return 武器基础伤害。
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	float GetHeroCurrentEquippedWeaponDamageAtLevel(float InLevel) const;

	//** 回调 **//
	virtual void OnHitTargetActor(AActor* HitActor) override; //当武器命中目标时
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor) override; //当武器离开目标时
};
