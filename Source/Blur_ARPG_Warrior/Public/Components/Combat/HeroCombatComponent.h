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

	//** 回调 **//
	virtual void OnHitTargetActor(AActor* HitActor); //当武器命中目标时
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor); //当武器离开目标时
};
