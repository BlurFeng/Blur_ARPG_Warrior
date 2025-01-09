// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UWarriorAbilitySystemComponent;
class UWarriorGameplayAbility;
class UGameplayEffect;

//启动数据资源，在启动时应用的内容。
UCLASS()
class BLUR_ARPG_WARRIOR_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

protected:

	//立即激活技能组。在启动后立刻赋予技能并激活，然后从技能将被移除。比如生成默认武器。
	UPROPERTY(EditDefaultsOnly, category = "StartUpData")
	TArray<TSubclassOf<UWarriorGameplayAbility>> ActivateOnGivenAbilities;

	//反应触发式技能。在特定条件下触发的技能。比如死亡。
	UPROPERTY(EditDefaultsOnly, category = "StartUpData")
	TArray<TSubclassOf<UWarriorGameplayAbility>> ReactiveAbilities;

	//启动时GE。在启动时直接应用到自身。比如设置最大血量和当前血量或持续恢复体力等。注意，如果GE本身有依赖关系，请注意配置的前后顺序。我们从数组的0位开始应用GE到自身。
	UPROPERTY(EditDefaultsOnly, category = "StartUpData")
	TArray<TSubclassOf<UGameplayEffect>> StartUpGameplayEffects;

	/// 赋予技能
	/// @param InAbilitiesToGive 
	/// @param InASCToGive 
	/// @param ApplyLevel 
	void GrantAbilities(const TArray<TSubclassOf<UWarriorGameplayAbility>>& InAbilitiesToGive, UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
