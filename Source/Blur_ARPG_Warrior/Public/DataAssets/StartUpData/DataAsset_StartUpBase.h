// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpBase.generated.h"

class UWarriorAbilitySystemComponent;
class UWarriorGameplayAbility;
/**
 * 启动数据资源
 */
UCLASS()
class BLUR_ARPG_WARRIOR_API UDataAsset_StartUpBase : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

protected:

	//激活给定能力组，在启动后立刻激活的能力。比如装备默认武器。
	UPROPERTY(EditDefaultsOnly, category = "StartUpData")
	TArray<TSubclassOf<UWarriorGameplayAbility>> ActivateOnGivenAbilities;

	//反应触发式能力。在特定条件下触发的能力。比如死亡。
	UPROPERTY(EditDefaultsOnly, category = "StartUpData")
	TArray<TSubclassOf<UWarriorGameplayAbility>> ReactiveAbilities;

	void GrantAbilities(const TArray<TSubclassOf<UWarriorGameplayAbility>>& InAbilitiesToGive, UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
};
