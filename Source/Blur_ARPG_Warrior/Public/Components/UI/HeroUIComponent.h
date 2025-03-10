// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponent.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"

#include "HeroUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquippedWeaponChangedDelegate, TSoftObjectPtr<UTexture2D>, SoftWeaponIcon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityIconSlotUpdatedDelegate, FGameplayTag, AbilityInputTag, TSoftObjectPtr<UMaterialInterface>, SoftAbilityIconMaterial);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAbilityCooldownBeginDelegate, FGameplayTag, AbilityInputTag, float, TotalCooldownTime, float, RemainingCooldownTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStoneInteractedDelegate, bool, bShouldDisplayInputKey);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTryActivateAbilityFailedDelegate, FGameplayTag, AbilityTag);

//英雄UI组件
UCLASS()
class BLUR_ARPG_WARRIOR_API UHeroUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()

public:
	//当前体力值变化时。
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentVitalityChanged;
	
	//当前怒气值变化时。
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentRageChanged;
	
	//当装备武器变化时。
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnEquippedWeaponChangedDelegate OnEquippedWeaponChangedDelegate;

	//当技能Icon槽更新时。
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnAbilityIconSlotUpdatedDelegate OnAbilityIconSlotUpdatedDelegate;

	//当技能CD开始时。
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnAbilityCooldownBeginDelegate OnAbilityCooldownBegin;

	//当和石头交互时。
	UPROPERTY(BlueprintAssignable)
	FOnStoneInteractedDelegate OnStoneInteracted;

	//当确认技能Cost是否足够时。
	UPROPERTY(BlueprintAssignable)
	FOnCheckCostOrCooldownDelegate OnCheckCost;

	//当确认技能Cooldown是否足够时。
	UPROPERTY(BlueprintAssignable)
	FOnCheckCostOrCooldownDelegate OnCheckCooldown;

	//当取消一个技能时。
	UPROPERTY(BlueprintAssignable)
	FOnCheckCostOrCooldownDelegate OnCancelAbility;

	//尝试激活一个技能失败时。
	UPROPERTY(BlueprintAssignable)
	FOnTryActivateAbilityFailedDelegate OnTryActivateAbilityFailed;
};
