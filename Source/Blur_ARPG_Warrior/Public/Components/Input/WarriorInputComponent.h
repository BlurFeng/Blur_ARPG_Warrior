// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "WarriorGameplayTags.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"

#include "WarriorInputComponent.generated.h"

// 输入组件。
UCLASS()
class BLUR_ARPG_WARRIOR_API UWarriorInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserObject, typename CallbackFunc>
	void BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig,
		const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func);

	template<class UserObject, typename CallbackFunc>
	void BindAbilityInputAction(
		const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject,
		CallbackFunc InputPressedFunc,  CallbackFunc InputReleasedFunc, CallbackFunc InputTriggeredFunc);
};

//Notes：C++ template
//C++的template模板是泛型编程的基础。允许提供非特定类型的参数。
//这里的class和typename关键字是等价的。怎么使用取决于习惯，通常在构建泛型方法时使用typename，构建泛型类时使用class。
//在这里，我想表示UserObject参数是一个类，CallbackFunc参数是一个回调方法。

/// 绑定输入事件
/// @tparam UserObject 
/// @tparam CallbackFunc 
/// @param InInputConfig 这是我们自定义的输入配置。根据配置找到对应的InputAction进行绑定。
/// @param InInputTag 根据InputTag我们会在InputConfig中找到对应的InputAction进行处理。
/// @param TriggerEvent 触发事件枚举，在指定的时机调用回调方法。
/// @param ContextObject 用户对象，通常是Character角色自身。
/// @param Func 回调方法，当输入事件触发时调用。
template <class UserObject, typename CallbackFunc>
void UWarriorInputComponent::BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig,
	const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func)
{
	//Notes：断言，确认InInputConfig参数是否有效。我们也可以使用check()进行断言，而checkf()方法允许传入自定义的TEXT信息。
	
	checkf(InInputConfig, TEXT("Input config data asset is null, can not proceed with binding."));

	//根据Tag找到对应的InputAction并绑定回调方法。
	if(UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
	{
		BindAction(FoundAction, TriggerEvent, ContextObject, Func);
	}
}

/// 绑定技能输入事件
/// @tparam UserObject 
/// @tparam CallbackFunc 
/// @param InInputConfig 这是我们自定义的输入配置。根据配置找到对应的InputAction进行绑定。
/// @param ContextObject 根据InputTag我们会在InputConfig中找到对应的InputAction进行处理。
/// @param InputPressedFunc 按下时回调方法。ETriggerEvent::Started。
/// @param InputReleasedFunc 松开时回调方法。ETriggerEvent::Completed。
/// @param InputTriggeredFunc 持续时回调方法。ETriggerEvent::Triggered。
template <class UserObject, typename CallbackFunc>
void UWarriorInputComponent::BindAbilityInputAction(
	const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject,
	CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc, CallbackFunc InputTriggeredFunc)
{
	checkf(InInputConfig, TEXT("Input config data asset is null, can not proceed with binding."));

	for (const FWarriorInputActionConfig& AbilityInputActionConfig : InInputConfig->AbilityInputActions)
	{
		if(!AbilityInputActionConfig.IsValid()) continue;

		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Started, ContextObject, InputPressedFunc, AbilityInputActionConfig.InputTag);
		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Completed, ContextObject, InputReleasedFunc, AbilityInputActionConfig.InputTag);

		//Notes：
		//需要按住的技能，绑定Triggered事件。否则我们不绑定。
		//绑定的IA需要在Triggers中配置Hold。否则Trigger事件会在Started后直接触发，导致一些UE原生的缓存数据在Trigger事件后清空。
		//已知的一个Bug：
		//比如通过 Started 触发一个GA。但GA可能不是这一帧立即执行的。之后触发了Trigger事件（即使Trigger事件里没有任何逻辑）。
		//然后当我们在GA中通过 GetLastMovementInputVector 获取移动方向时返回的是 Zero。此方法的缓存和输入系统相关，可能在Trigger事件触发时清空了缓存。
		
		if (AbilityInputActionConfig.InputTag.MatchesTag(WarriorGameplayTags::InputTag_MustBeHeld))
			BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Triggered, ContextObject, InputTriggeredFunc, AbilityInputActionConfig.InputTag);
	}
}
