// Blur Feng All Rights Reserved.


#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"

#include "WarriorDebugHelper.h"

#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "Characters/WarriorBaseCharacter.h"

UWarriorGameplayAbility::UWarriorGameplayAbility()
{
	OnCheckCostDelegate.AddUniqueDynamic(this, &ThisClass::OnCheckCost);
	OnCheckCooldownDelegate.AddUniqueDynamic(this, &ThisClass::OnCheckCooldown);
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

	OnCheckCostDelegate.Broadcast(bAllow, AbilityTags.First());
	
	return bAllow;
}

bool UWarriorGameplayAbility::CheckCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	const bool bAllow = Super::CheckCooldown(Handle, ActorInfo, OptionalRelevantTags);

	OnCheckCooldownDelegate.Broadcast(bAllow, AbilityTags.First());

	return bAllow;
}

void UWarriorGameplayAbility::OnCheckCost(const bool bAllow, const FGameplayTag AbilityTag)
{
}

void UWarriorGameplayAbility::OnCheckCooldown(const bool bAllow, const FGameplayTag AbilityTag)
{
}

bool UWarriorGameplayAbility::CheckConditionOnToggleableCancelAbility_Implementation()
{
	return true;
}

AWarriorBaseCharacter* UWarriorGameplayAbility::GetCharacterFromActorInfo()
{
	if(!CachedWarriorBaseCharacter.IsValid())
	{
		CachedWarriorBaseCharacter = Cast<AWarriorBaseCharacter>(CurrentActorInfo->AvatarActor);
	}
	
	return CachedWarriorBaseCharacter.IsValid() ? CachedWarriorBaseCharacter.Get() : nullptr;
}

UPawnCombatComponent* UWarriorGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
	//FindComponentByClass通过遍历查找返回第一个有效的目标。当存在多个UPawnCombatComponent的子类时可能无法按预期工作。
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}

UPawnUIComponent* UWarriorGameplayAbility::GetPawnUIComponentFromActorInfo()
{
	return GetCharacterFromActorInfo()->GetPawnUIComponent();
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

// Notes：这里不适用const方法，是应为const后方法在蓝图中是非执行节点，没有Exec节点可以连接。
FActiveGameplayEffectHandle UWarriorGameplayAbility::BP_ApplyEffectSpecHandleTarget(AActor* TargetActor,
	const FGameplayEffectSpecHandle& InSpecHandle, EWarriorSuccessType& OutSuccessType)
{
	const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleTarget(TargetActor, InSpecHandle);

	OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied() ? EWarriorSuccessType::Successful : EWarriorSuccessType::Failed;

	return ActiveGameplayEffectHandle;
}

// Tips：这里不为方法添加 const，否则此方法在蓝图中为成为 Pure 相同的节点，没有执行引脚。
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

FGameplayEffectSpecHandle UWarriorGameplayAbility::MakeSpecHandle(const TSubclassOf<UGameplayEffect> EffectClass) const
{
	//创建EffectContext。这是上下文信息，帮助技能系统获取执行技能所需的信息。
	//比如在使用UGameplayEffectExecutionCalculation类计算伤害时，通过Context我们能获取我们需要的内容。
	//搜索 UGEExecCalc_DamageTaken::Execute_Implementation 来查看其中一个计算用类。
	FGameplayEffectContextHandle ContextHandle = GetWarriorAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());
	
	//创建FGameplayEffectSpecHandle
	FGameplayEffectSpecHandle EffectSpecHandle = GetWarriorAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
		);

	return EffectSpecHandle;
}

FGameplayEffectSpecHandle UWarriorGameplayAbility::MakeSpecHandleSetByCallerMagnitude(
	const TSubclassOf<UGameplayEffect> EffectClass, const FGameplayTag GameplayTag, const float Magnitude) const
{
	//创建GE查询句柄。
	FGameplayEffectSpecHandle EffectSpecHandle = MakeSpecHandle(EffectClass);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(GameplayTag, Magnitude);

	return EffectSpecHandle;
}

//Notes：蓝图方法的参数和 & 关键字。
//在蓝图中，像 TSubclassOf<UGameplayEffect> 这样的参数，如果想使用UE的资源映射直接选择一个参数，不应当使用 & 关键字。
//否则你必须创建一个成员字段然后传给 TSubclassOf<UGameplayEffect> 参数。

FGameplayEffectSpecHandle UWarriorGameplayAbility::MakeDamageEffectSpecHandle(
	const TSubclassOf<UGameplayEffect> EffectClass, const float InBaseDamage, const float InBaseDamageMultiplyCoefficient, const int32 DamageIncreaseCount, const float DamageIncreaseCoefficient) const
{
	check(EffectClass);

	//创建GE查询句柄。
	FGameplayEffectSpecHandle EffectSpecHandle = MakeSpecHandle(EffectClass);

	//Notes：我们可以将数据缓存到Data并在之后使用，存储的数据通过Tag查询。

	//Tips：SetByCallerTagMagnitudes 缓存到Data中的值用于计算。搜索 SetByCallerTagMagnitudes 找到使用处。
	//缓存基础伤害到Data。用于之后的计算。
	EffectSpecHandle.Data->SetSetByCallerMagnitude(WarriorGameplayTags::Shared_SetByCaller_Attack_BaseDamage, InBaseDamage);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(WarriorGameplayTags::Shared_SetByCaller_Attack_BaseDamage_MultiplyCoefficient, InBaseDamageMultiplyCoefficient);

	//缓存增伤计数和系数到Data。用于之后的计算。
	EffectSpecHandle.Data->SetSetByCallerMagnitude(WarriorGameplayTags::Shared_SetByCaller_Attack_DamageIncreaseCount, DamageIncreaseCount);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(WarriorGameplayTags::Shared_SetByCaller_Attack_DamageIncreaseCoefficient, DamageIncreaseCoefficient);
	
	return EffectSpecHandle;
}

FGameplayEffectSpecHandle UWarriorGameplayAbility::MakeDamageEffectSpecHandleByScalableFloat(
	const TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InBaseDamageScalableFloat,
	const float InBaseDamageMultiplyCoefficient, const int32 DamageIncreaseCount,
	const float DamageIncreaseCoefficient) const
{
	return MakeDamageEffectSpecHandle(EffectClass, GetScalableFloatValueAtLevel(InBaseDamageScalableFloat), InBaseDamageMultiplyCoefficient, DamageIncreaseCount, DamageIncreaseCoefficient);
}

float UWarriorGameplayAbility::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat) const
{
	return UWarriorFunctionLibrary::GetScalableFloatValueAtLevel(InScalableFloat, GetAbilityLevel());
}
