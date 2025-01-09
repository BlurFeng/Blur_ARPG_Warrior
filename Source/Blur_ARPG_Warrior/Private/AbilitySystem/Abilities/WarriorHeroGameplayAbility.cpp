// Blur Feng All Rights Reserved.


#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"

#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Characters/WarriorHeroCharacter.h"
#include "Controllers/WarriorHeroController.h"
#include "Components/UI/HeroUIComponent.h"

void UWarriorHeroGameplayAbility::OnCheckCost(const bool bAllow, const FGameplayTag AbilityTag)
{
	Super::OnCheckCost(bAllow, AbilityTag);

	if (GetHeroUIComponentFromActorInfo())
		GetHeroUIComponentFromActorInfo()->OnCheckCost.Broadcast(bAllow, AbilityTag);
}

void UWarriorHeroGameplayAbility::OnCheckCooldown(const bool bAllow, const FGameplayTag AbilityTag)
{
	Super::OnCheckCooldown(bAllow, AbilityTag);

	if (GetHeroUIComponentFromActorInfo())
		GetHeroUIComponentFromActorInfo()->OnCheckCooldown.Broadcast(bAllow, AbilityTag);
}

AWarriorHeroCharacter* UWarriorHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
	if (!CurrentActorInfo) return nullptr;
	
	if(!CachedWarriorHeroCharacter.IsValid())
	{
		CachedWarriorHeroCharacter = Cast<AWarriorHeroCharacter>(CurrentActorInfo->AvatarActor);
	}
	
	return CachedWarriorHeroCharacter.IsValid() ? CachedWarriorHeroCharacter.Get() : nullptr;
}

AWarriorHeroController* UWarriorHeroGameplayAbility::GetHeroControllerFromActorInfo()
{
	if(!CachedWarriorHeroController.IsValid())
	{
		CachedWarriorHeroController = Cast<AWarriorHeroController>(CurrentActorInfo->PlayerController);
	}

	return CachedWarriorHeroController.IsValid() ? CachedWarriorHeroController.Get() : nullptr;
}

UHeroCombatComponent* UWarriorHeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
	const AWarriorHeroCharacter* HeroChar = GetHeroCharacterFromActorInfo();
	return HeroChar ? HeroChar->GetHeroCombatComponent() : nullptr;
}

UHeroUIComponent* UWarriorHeroGameplayAbility::GetHeroUIComponentFromActorInfo()
{
	if (GetHeroCharacterFromActorInfo())
		return GetHeroCharacterFromActorInfo()->GetHeroUIComponent();
	return nullptr;
}

bool UWarriorHeroGameplayAbility::GetAbilityRemainingCooldownByTag(FGameplayTag InCooldownTag, float& TotalCooldownTime,
	float& RemainingCooldownTime) const
{
	check(InCooldownTag.IsValid());

	const FGameplayEffectQuery CooldownQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCooldownTag.GetSingleTagContainer());
	
	const TArray<TPair<float, float>> TimeRemainingAndDuration = GetAbilitySystemComponentFromActorInfo()->GetActiveEffectsTimeRemainingAndDuration(CooldownQuery);

	if (!TimeRemainingAndDuration.IsEmpty())
	{
		RemainingCooldownTime = TimeRemainingAndDuration[0].Key;
		TotalCooldownTime = TimeRemainingAndDuration[0].Value;
	}

	return RemainingCooldownTime > 0.f;
}
