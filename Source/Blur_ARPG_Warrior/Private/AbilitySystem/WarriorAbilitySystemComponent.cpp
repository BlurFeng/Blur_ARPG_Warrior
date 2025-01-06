// Blur Feng All Rights Reserved.


#include "AbilitySystem/WarriorAbilitySystemComponent.h"

#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "WarriorTypes/WarriorStructTypes.h"

void UWarriorAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		//确认输入的技能是否存在，通过对比InInputTag。此Tag应当在启动时被添加。
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag)) continue;

		//切换类技能在每次按下时，在触发和取消之间切换。
		if (InInputTag.MatchesTag(WarriorGameplayTags::InputTag_Toggleable) && AbilitySpec.IsActive())
		{
			//TODO：使用配置打断技能来取消此技能，这样能够通过GameplayTag的配置来确认哪些情况下不能取消技能。
			//比如使用愤怒技能后，必须等待愤怒技能成功激活后才能取消。或者愤怒值低于一半后才能取消等。
			//尝试取消技能
			CancelAbilityHandle(AbilitySpec.Handle);
		}
		//技能触发。所有的技能触发都在此调用。
		else
		{
			//尝试触发技能。
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UWarriorAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	//如果是必须持续按住的技能，在松开时取消技能。
	if (!InInputTag.IsValid()) return;

	if (InInputTag.MatchesTag(WarriorGameplayTags::InputTag_MustBeHeld))
	{
		for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
		{
			//取消正在激活中的技能。
			if (AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag) && AbilitySpec.IsActive())
			{
				CancelAbilityHandle(AbilitySpec.Handle);
			}
		}
	}
}

void UWarriorAbilitySystemComponent::GrantHeroWeaponAbilities(
	const TArray<FWarriorHeroAbilitySet> InDefaultWeaponAbilities, const TArray<FWarriorHeroSpecialAbilitySet> InSpecialWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
	if (InDefaultWeaponAbilities.IsEmpty()) return;

	//默认武器技能。
	for (const FWarriorHeroAbilitySet& AbilitySet : InDefaultWeaponAbilities)
	{
		if (!AbilitySet.IsValid()) continue;
		
		FGameplayAbilitySpec AbilitySpec =
			UWarriorFunctionLibrary::NativeGetGameplayAbilitySpec(AbilitySet.AbilityToGrant, GetAvatarActor(), ApplyLevel, AbilitySet.InputTag);

		//缓存赋予技能Handle，方便之后用于移除技能等操作。
		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}

	//特殊武器技能。
	for (const FWarriorHeroSpecialAbilitySet& AbilitySet : InSpecialWeaponAbilities)
	{
		if (!AbilitySet.IsValid()) continue;
		
		FGameplayAbilitySpec AbilitySpec =
			UWarriorFunctionLibrary::NativeGetGameplayAbilitySpec(AbilitySet.AbilityToGrant, GetAvatarActor(), ApplyLevel, AbilitySet.InputTag);

		//缓存赋予技能Handle，方便之后用于移除技能等操作。
		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}
}

void UWarriorAbilitySystemComponent::RemoveGrantedHeroWeaponAbilities(TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove)
{
	if (InSpecHandlesToRemove.IsEmpty()) return;

	for (const FGameplayAbilitySpecHandle& SpecHandle : InSpecHandlesToRemove)
	{
		if(!SpecHandle.IsValid()) continue;
		
		ClearAbility(SpecHandle);
	}

	InSpecHandlesToRemove.Empty();
}

bool UWarriorAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate)
{
	check(AbilityTagToActivate.IsValid());

	//查询所有符合条件的技能。
	TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagToActivate.GetSingleTagContainer(), FoundAbilitySpecs);

	if (!FoundAbilitySpecs.IsEmpty())
	{
		//随机触发一个技能。
		const int32 RandomAbilityIndex = FMath::RandRange(0, FoundAbilitySpecs.Num() - 1);
		FGameplayAbilitySpec* AbilitySpecToActivate = FoundAbilitySpecs[RandomAbilityIndex];

		check(AbilitySpecToActivate);

		if (!AbilitySpecToActivate->IsActive())
		{
			return TryActivateAbility(AbilitySpecToActivate->Handle);
		}
	}

	return false;
}
