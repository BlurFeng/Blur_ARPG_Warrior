// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/WarriorWeaponBase.h"
#include "WarriorTypes/WarriorStructTypes.h"
#include "GameplayAbilitySpecHandle.h"

#include "WarriorHeroWeapon.generated.h"

//角色武器基类
UCLASS()
class BLUR_ARPG_WARRIOR_API AWarriorHeroWeapon : public AWarriorWeaponBase
{
	GENERATED_BODY()

public:
	//英雄武器数据
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FWarriorHeroWeaponData HeroWeaponData;

	/// 缓存 赋予技能SpecHandle组
	/// @param InSpecHandles 
	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles);

	/// 获取 赋予技能SpecHandle组
	/// @return 
	UFUNCTION(BlueprintCallable)
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const;
private:
	
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
};
