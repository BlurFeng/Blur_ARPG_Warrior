// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "WarriorAbilitySystemComponent.generated.h"

struct FWarriorHeroSpecialAbilitySet;
struct FWarriorHeroAbilitySet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityInputPressedDelegate, FGameplayTag, InputTag);

//技能系统组件
UCLASS()
class BLUR_ARPG_WARRIOR_API UWarriorAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UWarriorAbilitySystemComponent();

#pragma region Delegate
	// 当获得技能输入按下时。
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "On Ability Input Pressed"))
	FOnAbilityInputPressedDelegate OnAbilityInputPressedDelegate;
#pragma endregion
	
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);
	void OnAbilityInputTriggered(const FGameplayTag& InInputTag);

	/// 赋予英雄武器技能。在装备武器时使用此方法为角色赋予装备技能。
	/// @param InDefaultWeaponAbilities
	/// @param InSpecialWeaponAbilities 
	/// @param ApplyLevel 
	/// @param OutGrantedAbilitySpecHandles 
	UFUNCTION(BlueprintCallable, Category = "Warrior|Ability", meta = (ApplyLevel = 1))
	void GrantHeroWeaponAbilities(const TArray<FWarriorHeroAbilitySet> InDefaultWeaponAbilities, const TArray<FWarriorHeroSpecialAbilitySet> InSpecialWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);

	//Notes：使用UPARAM(ref)宏告知UE此参数不是输出参数。
	//只添加 & 时，此参数进行引用传递。在UE蓝图中，此参数还会被认为是输出参数。
	//在作为输入参数时就要添加 UPARAM(ref) 宏。

	/// 移除赋予的武器技能。在卸下装备时使用此方法移除装备技能。
	/// @param InSpecHandlesToRemove 
	UFUNCTION(BlueprintCallable, Category = "Warrior|Ability")
	void RemoveGrantedHeroWeaponAbilities(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove);

	/// 尝试激活技能，根据Tag。
	/// 查询所有符合Tag要求的技能，并随机激活其中一个。
	/// @param AbilityTagToActivate 
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Warrior|Ability")
	bool TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate);

private:
	UPROPERTY()
	FGameplayTag CachedMustBeHeldGameplayAbilityInputTag;
	UPROPERTY()
	FGameplayAbilitySpecHandle CachedMustBeHeldFGameplayAbilitySpecHandle;
};
