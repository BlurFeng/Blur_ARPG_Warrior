// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"

#include "WarriorInputComponent.generated.h"

/**
 * 战士输入组件
 */
UCLASS()
class BLUR_ARPG_WARRIOR_API UWarriorInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserObject, typename CallbackFunc>
	void BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig,
		const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func);

	template<class UserObject, typename CallbackFunc>
	void BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject, CallbackFunc InputPressedFunc,  CallbackFunc InputReleasedFunc);
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
template <class UserObject, typename CallbackFunc>
void UWarriorInputComponent::BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig,
	UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc)
{
	checkf(InInputConfig, TEXT("Input config data asset is null, can not proceed with binding."));

	for (const FWarriorInputActionConfig& AbilityInputActionConfig : InInputConfig->AbilityInputActions)
	{
		if(!AbilityInputActionConfig.IsValid()) continue;

		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Started, ContextObject, InputPressedFunc, AbilityInputActionConfig.InputTag);
		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Completed, ContextObject, InputReleasedFunc, AbilityInputActionConfig.InputTag);
	}
}
