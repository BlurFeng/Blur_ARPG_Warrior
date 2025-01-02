// Blur Feng All Rights Reserved.


#include "Components/Combat/EnemyCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "WarriorDebugHelper.h"
#include "WarriorGameplayTags.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	Super::OnHitTargetActor(HitActor);

	//确认是否已经对击中目标造成过效果.
	if (OverlappedActors.Contains(HitActor)) return;
	OverlappedActors.Add(HitActor);

	bool bIsValidBlock = false;

	const bool bIsPlayerBlocking = false;
	const bool bIsMyAttackUnBlockable = false;

	if (bIsPlayerBlocking && !bIsMyAttackUnBlockable)
	{
		//TODO： check if the block is valid
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;

	if (bIsValidBlock)
	{
		//TODO: Handle successful block
	}
	else
	{
		//发送击中事件。
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			WarriorGameplayTags::Shared_Event_MeleeHit,
			EventData);
	}
}
