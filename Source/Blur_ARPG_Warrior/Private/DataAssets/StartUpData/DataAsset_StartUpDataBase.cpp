// Blur Feng All Rights Reserved.


#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"

#include "WarriorFunctionLibrary.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"


void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	check(InASCToGive);

	//初始化技能
	GrantAbilities(ActivateOnGivenAbilities, InASCToGive, ApplyLevel); //在赋予技能后立即激活并清除。比如生成默认武器。
	GrantAbilities(ReactiveAbilities, InASCToGive, ApplyLevel); //反应式技能，比如死亡。

	//应用启动时GameplayEffect到自身。比如设置最大血量和当前血量的GE。
	if(!StartUpGameplayEffects.IsEmpty())
	{
		for (const TSubclassOf<UGameplayEffect>& EffectClass : StartUpGameplayEffects)
		{
			if(!EffectClass) continue;

			//通过 GetDefaultObject() 获取默认对象实例。此方法在UE中非常常用，避免我们自己多次创建相同的Object。
			const UGameplayEffect* EffectCDO = EffectClass->GetDefaultObject<UGameplayEffect>();
			InASCToGive->ApplyGameplayEffectToSelf(
				EffectCDO,
				ApplyLevel,
				InASCToGive->MakeEffectContext()
				);
		}
	}
}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UWarriorGameplayAbility>>& InAbilitiesToGive,
	UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	if(InAbilitiesToGive.IsEmpty()) return;

	//在启动时，将配置的GameplayAbility给到AvatarActor的AbilitySystemComponent。
	for(const TSubclassOf<UWarriorGameplayAbility>& Ability : InAbilitiesToGive)
	{
		if(!Ability) continue;
		
		FGameplayAbilitySpec AbilitySpec =
			UWarriorFunctionLibrary::NativeGetGameplayAbilitySpec(Ability, InASCToGive->GetAvatarActor(), ApplyLevel);
		
		//给角色（技能组件）技能，只有经过GiveAbility()赋予的技能才能使用。
		//Tips:这里只是启动配置时的技能，攻击等技能在子类UDataAsset_HeroStartUp等里实现
		InASCToGive->GiveAbility(AbilitySpec);
	}
}
