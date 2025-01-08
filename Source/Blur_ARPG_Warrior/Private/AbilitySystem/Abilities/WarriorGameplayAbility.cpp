// Blur Feng All Rights Reserved.


#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "WarriorDebugHelper.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "Components/UI/HeroUIComponent.h"
#include "Interfaces/PawnUIInterface.h"

UWarriorGameplayAbility::UWarriorGameplayAbility()
{
	OnCheckCostDelegate.AddUniqueDynamic(this, &ThisClass::OnCheckCost);
	OnCheckCooldownDelegate.AddUniqueDynamic(this, &ThisClass::OnCheckCooldown);
}

void UWarriorGameplayAbility::OnCheckCost(bool bAllow, FGameplayTag AbilityTag)
{
	if (GetPawnUIInterface().IsValid() && GetPawnUIInterface()->GetHeroUIComponent())
	{
		GetPawnUIInterface()->GetHeroUIComponent()->OnCheckCost.Broadcast(bAllow, AbilityTag);
	}
}

void UWarriorGameplayAbility::OnCheckCooldown(bool bAllow, FGameplayTag AbilityTag)
{
	if (GetPawnUIInterface().IsValid() && GetPawnUIInterface()->GetHeroUIComponent())
	{
		GetPawnUIInterface()->GetHeroUIComponent()->OnCheckCooldown.Broadcast(bAllow, AbilityTag);
	}
}

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

bool UWarriorGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	const bool bAllow = Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);

	OnCheckCostDelegate.Broadcast(bAllow, AbilityTags.GetByIndex(0));
	
	return bAllow;
}

bool UWarriorGameplayAbility::CheckCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	const bool bAllow = Super::CheckCooldown(Handle, ActorInfo, OptionalRelevantTags);

	OnCheckCooldownDelegate.Broadcast(bAllow, AbilityTags.GetByIndex(0));

	return bAllow;
}

UPawnCombatComponent* UWarriorGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
	//FindComponentByClass通过遍历查找返回第一个有效的目标。当存在多个UPawnCombatComponent的子类时可能无法按预期工作。
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}

UWarriorAbilitySystemComponent* UWarriorGameplayAbility::GetWarriorAbilitySystemComponentFromActorInfo() const
{
	return Cast<UWarriorAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

FActiveGameplayEffectHandle UWarriorGameplayAbility::NativeApplyEffectSpecHandleTarget(AActor* TargetActor,
	const FGameplayEffectSpecHandle& InSpecHandle) const
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	return GetWarriorAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*InSpecHandle.Data,
		TargetASC
		);
}

//Notes：这里不适用const方法，是应为const后方法在蓝图中是非执行节点，没有Exec节点可以连接。
FActiveGameplayEffectHandle UWarriorGameplayAbility::BP_ApplyEffectSpecHandleTarget(AActor* TargetActor,
	const FGameplayEffectSpecHandle& InSpecHandle, EWarriorSuccessType& OutSuccessType)
{
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleTarget(TargetActor, InSpecHandle);

	OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied() ? EWarriorSuccessType::Successful : EWarriorSuccessType::Failed;

	return ActiveGameplayEffectHandle;
}

void UWarriorGameplayAbility::ApplyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle,
	const TArray<FHitResult>& InHitResults)
{
	if (InHitResults.IsEmpty()) return;
	
	const APawn* OwningPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
	if (!OwningPawn) return;
	
	for (const FHitResult& HitResult : InHitResults)
	{
		if (APawn* HitPawn = Cast<APawn>(HitResult.GetActor()))
		{
			if (UWarriorFunctionLibrary::IsTargetPawnHostile(OwningPawn, HitPawn))
			{
				//对目标造成伤害影响。
				FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleTarget(HitPawn, InSpecHandle);
				
				if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
				{
					FGameplayEventData Data;
					Data.Instigator = OwningPawn;
					Data.Target = HitPawn;

					//目标被击中事件。
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
						HitPawn,
						WarriorGameplayTags::Shared_Event_HitReact,
						Data);
				}
			}
		}
	}
}

TWeakInterfacePtr<IPawnUIInterface> UWarriorGameplayAbility::GetPawnUIInterface()
{
	//Tips：CachedPawnUIInterface缓存在GA的InstancingPolicy不是Actor时。
	//如果TA没有成功激活，CachedPawnUIInterface是无法正常获取到的。
	
	//获取人物UI接口
	if(!CachedPawnUIInterface.IsValid())
	{
		//使用弱指针缓存UI接口
		CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(GetAvatarActorFromActorInfo());
		//CachedPawnUIInterface = Cast<IPawnUIInterface>(Data.Target.GetAvatarActor()); //等价方法
	}

	return CachedPawnUIInterface;
}