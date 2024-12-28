// Blur Feng All Rights Reserved.


#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"

void UWarriorGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	//技能激活策略为给定形式
	if(AbilityActivationPolicy == EWarriorAbilityActivationPolicy::OnGiven)
	{
		if(ActorInfo && !Spec.IsActive())
		{
			//尝试激活技能
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UWarriorGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	//技能激活策略为给定形式
	if(AbilityActivationPolicy == EWarriorAbilityActivationPolicy::OnGiven)
	{
		if(ActorInfo)
		{
			//立即结束清除此技能
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}
