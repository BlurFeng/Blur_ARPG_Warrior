// Blur Feng All Rights Reserved.


#include "AbilitySystem/WarriorAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "WarriorDebugHelper.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "Components/UI/HeroUIComponent.h"
#include "Interfaces/PawnUIInterface.h"
#include "WarriorTypes/WarriorStructTypes.h"
#include "WarriorTypes/WarriorEnumTypes.h"

UWarriorAbilitySystemComponent::UWarriorAbilitySystemComponent()
{
}

void UWarriorAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;

	// Debug::Print(TEXT("OnAbilityInputPressed 1"));

	// 当有技能输入时广播。无论是否成功激活。此多播委托主要告知玩家进行了技能输入行为。
	OnAbilityInputPressedDelegate.Broadcast(InInputTag);

	// 确认输入类型。
	EWarriorInputType InputType = EWarriorInputType::Normal;
	if (InInputTag.MatchesTag(WarriorGameplayTags::InputTag_Toggleable))
		InputType = EWarriorInputType::Toggleable;
	else if (InInputTag.MatchesTag(WarriorGameplayTags::InputTag_MustBeHeld))
		InputType = EWarriorInputType::MustBeHeld;

	// 搜索对应 InInputTag 的技能。
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// Tips：InInputTag 通过 UWarriorFunctionLibrary::NativeGetGameplayAbilitySpec 方法在赋予技能时添加到 DynamicAbilityTags。
		
		// 确认输入的技能是否存在，通过对比InInputTag。此Tag应当在启动时被添加。
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag)) continue;

		bool bTryActivateAbility = false;

		if (InputType == EWarriorInputType::Normal)
		{
			if (AbilitySpec.IsActive())
			{
				FGameplayEventData Data;
				Data.Instigator = GetAvatarActor();
				Data.Target = GetAvatarActor();
				// 在技能激活时重复输入。这是一个公用的事件，可以用来构建连击技能。
				// Tips：这里我们会先确认 InInputTag ，然后找到对应的GA。保证了事件传输的准确性。
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
					GetAvatarActor(),
					WarriorGameplayTags::Player_Event_RepeatInputWhenAbilityActive,
					Data);
			}
			else
			{
				bTryActivateAbility = true;
			}
		}
		// 切换形式的技能。在激活和取消之间切换。比如可切换的愤怒状态。
		else if (InputType == EWarriorInputType::Toggleable)
		{
			if (!AbilitySpec.IsActive())
			{
				bTryActivateAbility = true;
			}
			else
			{
				bool Allow = false; // 允许取消技能。
				// 切换形式的技能如果想在取消时进行确认，可重写 CheckCondition() 方法。不重写时默认返回true。
				// 且需要技能是Actor形式的，否则 GetPrimaryInstance() 无法获得他的实例。
				if (UWarriorGameplayAbility* WarriorGameplayAbility = Cast<UWarriorGameplayAbility>(AbilitySpec.GetPrimaryInstance()))
				{
					// 确认自定义的确认条件是否允许取消。
					if (WarriorGameplayAbility->CheckConditionOnToggleableCancelAbility())
					{
						Allow = true;
					}
					
					// Debug::Print(FString::Printf(TEXT("Cancel Ability, Name: %s"), *AbilitySpec.Ability.GetName()));
				}
				// 直接取消技能。
				else
				{
					Allow = true;
				}
				
				if (Allow)
				{
					CancelAbilityHandle(AbilitySpec.Handle);
				}

				if (const IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(GetAvatarActor()))
				{
					if (PawnUIInterface->GetHeroUIComponent() && AbilitySpec.GetPrimaryInstance())
						PawnUIInterface->GetHeroUIComponent()->OnCancelAbility.Broadcast(Allow, AbilitySpec.GetPrimaryInstance()->AbilityTags.First());
				}
			}
		}
		// 必须持续按住的技能，松开时取消。比如防御。
		else if (InputType == EWarriorInputType::MustBeHeld)
		{
			CachedMustBeHeldGameplayAbilityInputTag = InInputTag;
			CachedMustBeHeldFGameplayAbilitySpecHandle = AbilitySpec.Handle;

			bTryActivateAbility = true;
		}

		if (bTryActivateAbility)
		{
			// 尝试触发技能。
			if (!TryActivateAbility(AbilitySpec.Handle))
			{
				if (const IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(GetAvatarActor()))
				{
					if (PawnUIInterface->GetHeroUIComponent() && AbilitySpec.GetPrimaryInstance())
						PawnUIInterface->GetHeroUIComponent()->OnTryActivateAbilityFailed.Broadcast(AbilitySpec.GetPrimaryInstance()->AbilityTags.First());
				}
			}
		}

		// 允许一个InputTag对应多个GA。
		//break;
	}
}

void UWarriorAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;
	
	//如果是必须持续按住的技能，在松开时取消技能。
	if (InInputTag.MatchesTag(WarriorGameplayTags::InputTag_MustBeHeld))
	{
		for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
		{
			//取消正在激活中的技能。
			if (AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag) && AbilitySpec.IsActive())
			{
				//清空缓存的需要持续按住触发的技能Spec。
				CachedMustBeHeldGameplayAbilityInputTag = WarriorGameplayTags::None; //FGameplayTag::EmptyTag;
				CancelAbilityHandle(AbilitySpec.Handle);

				//Debug::Print(FString::Printf(TEXT("Cancel Ability, Name: %s"), *AbilitySpec.Ability.GetName()));
				break;
			}
		}
	}
}

void UWarriorAbilitySystemComponent::OnAbilityInputTriggered(const FGameplayTag& InInputTag)
{
	//在持续按住的技能一直按住时，尝试在技能未激活时（被打断）再触发。
	if (CachedMustBeHeldGameplayAbilityInputTag == InInputTag)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(CachedMustBeHeldFGameplayAbilitySpecHandle))
		{
			if (!AbilitySpec->IsActive())
				TryActivateAbility(AbilitySpec->Handle);
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
		const FGameplayAbilitySpec* AbilitySpecToActivate = FoundAbilitySpecs[RandomAbilityIndex];

		check(AbilitySpecToActivate);

		if (!AbilitySpecToActivate->IsActive())
		{
			return TryActivateAbility(AbilitySpecToActivate->Handle);
		}
	}

	return false;
}
