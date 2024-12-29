// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponentBase.h"
#include "GameplayTagContainer.h"

#include "PawnCombatComponent.generated.h"

class AWarriorWeaponBase;
//人物战斗组件基类，角色或敌人的战斗组件由此类派生
UCLASS()
class BLUR_ARPG_WARRIOR_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()
	
public:
	//当前装备武器Tag。应当在使用“装备武器”技能Ability时修改。
	UPROPERTY(BlueprintReadWrite, Category = "Warrior|Combat")
	FGameplayTag CurrentEquippedWeaponTag;

	/// 注册一个生成武器。
	/// @param InWeaponTagToRegister 武器Tag。
	/// @param InWeaponToRegister 生成的武器。
	/// @param bRegisterAsEquippedWeapon 注册的武器装备到自身。
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AWarriorWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon = false);

	/// 获取角色当前持有的武器，通过Tag。
	/// @param InWeaponTagToGet 武器Tag。
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWarriorWeaponBase* GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

	/// 获取角色当前装备的武器。
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWarriorWeaponBase* GetCharacterCurrentEquippedWeapon() const;
	
private:
	TMap<FGameplayTag, AWarriorWeaponBase*> CharacterCarriedWeaponsMap;
};
