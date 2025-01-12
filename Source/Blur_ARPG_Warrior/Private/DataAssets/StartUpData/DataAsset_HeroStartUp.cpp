// Blur Feng All Rights Reserved.


#include "DataAssets/StartUpData/DataAsset_HeroStartUp.h"

#include "WarriorFunctionLibrary.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "WarriorTypes/WarriorStructTypes.h"

void UDataAsset_HeroStartUp::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

	//Tips：这里的代码实现和GrantAbilities()很像，但GrantAbilities()方法直接赋予GameplayAbility。
	//而这里我们通过 FWarriorHeroAbilitySet 来赋予 GameplayAbility 的同时，还设置了 InputTag。方便之后 UBlurEnhancedInputComponent 以及相关输入系统流程的使用。

	for (const FWarriorHeroAbilitySet& AbilitySet : HeroStartUpAbilitySets)
	{
		if(!AbilitySet.IsValid()) continue;

		FGameplayAbilitySpec AbilitySpec =
			UWarriorFunctionLibrary::NativeGetGameplayAbilitySpec(AbilitySet.AbilityToGrant, InASCToGive->GetAvatarActor(), ApplyLevel, AbilitySet.InputTag);

		//Tips：这里赋予的技能是需要玩家输入进行使用的技能。
		//给角色（技能组件）技能，只有经过GiveAbility()赋予的技能才能使用。
		InASCToGive->GiveAbility(AbilitySpec);
	}
}

#if WITH_EDITOR
void UDataAsset_HeroStartUp::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	//UBlueprint* Blueprint = UBlueprint::GetBlueprintFromClass(GetClass());

	if (PropertyThatChanged && PropertyThatChanged->GetFName() == GET_MEMBER_NAME_CHECKED(UDataAsset_HeroStartUp, HeroStartUpAbilitySets))
	{
		for (FWarriorHeroAbilitySet& Item : HeroStartUpAbilitySets)
		{
			Item.PostEditChangeProperty();
		}
	}
}
#endif