// Blur Feng All Rights Reserved.


#include "AbilitySystem/WarriorAbilitySystemComponent.h"

void UWarriorAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if(!InInputTag.IsValid()) return;

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		//确认输入的技能是否存在，通过对比InInputTag。此Tag应当在启动时被添加。
		if(!AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag)) continue;

		//尝试触发技能
		TryActivateAbility(AbilitySpec.Handle);
	}
}

void UWarriorAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	
}
