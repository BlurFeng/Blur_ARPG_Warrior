// Blur Feng All Rights Reserved.


#include "DataAssets/StartUpData/DataAsset_HeroStartUp.h"

#include "WarriorFunctionLibrary.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "WarriorTypes/WarriorStructTypes.h"

void UDataAsset_HeroStartUp::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

	//Tips：这里的代码实现和GrantAbilities()很像，但GrantAbilities()方法直接赋予GameplayAbility。
	//而这里我们通过FWarriorHeroAbilitySet来赋予GameplayAbility的同时，还设置相关的Tag参数。
	//因为HeroStartUpAbilitySets的技能是运行时使用的主动或被动技能，有更复杂的状态机制。

	for (const FWarriorHeroAbilitySet& AbilitySet : HeroStartUpAbilitySets)
	{
		if(!AbilitySet.IsValid()) continue;

		FGameplayAbilitySpec AbilitySpec =
			UWarriorFunctionLibrary::NativeGetGameplayAbilitySpec(AbilitySet.AbilityToGrant, InASCToGive->GetAvatarActor(), ApplyLevel, AbilitySet.InputTag);
		
		//给角色（技能组件）技能，只有经过GiveAbility()赋予的技能才能使用。
		//Tips：这里赋予的技能是类似装备武器，攻击能需要输入主动触发的技能。
		InASCToGive->GiveAbility(AbilitySpec);
	}
}

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
