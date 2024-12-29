// Blur Feng All Rights Reserved.


#include "DataAssets/StartUpData/DataAsset_HeroStartUp.h"

#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"

bool FWarriorHeroAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}

void UDataAsset_HeroStartUp::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

	//Tips：这里的代码实现和GrantAbilities()很像，但GrantAbilities()方法直接赋予GameplayAbility。
	//而这里我们通过FWarriorHeroAbilitySet来赋予GameplayAbility的同时，还设置相关的Tag参数。
	//因为HeroStartUpAbilitySets的技能是运行时使用的主动或被动技能，有更复杂的状态机制。

	for (const FWarriorHeroAbilitySet& AbilitySet : HeroStartUpAbilitySets)
	{
		if(!AbilitySet.IsValid()) continue;

		// Gameplay Ability Specification 用于描述Ability技能的详细信息。
		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

		//给角色（技能组件）技能，只有经过GiveAbility()赋予的技能才能使用。
		//Tips：这里赋予的技能是类似装备武器，攻击能需要输入主动触发的技能。
		InASCToGive->GiveAbility(AbilitySpec);
	}
}
