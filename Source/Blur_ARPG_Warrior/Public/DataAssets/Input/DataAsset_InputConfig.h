// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "DataAsset_InputConfig.generated.h"

class UInputAction;
class UInputMappingContext;

// 输入事件配置结构体。
USTRUCT(BlueprintType)
struct FWarriorInputActionConfig
{
	GENERATED_BODY() // UE生成相关宏，否则无法生成。

public:

	// Notes：通过 meta = (Categories = "InputTag")，我们限定可选的Tag只能是InputTag.之下的Tag。
	
	// 输入行为对应的Tag。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	// 输入事件类。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAction;

	bool IsValid() const
	{
		return InputTag.IsValid() && InputAction;
	}
};

// 输入配置资源。
// 我们使用UE5新的Enhanced Input增强输入系统来进行输入操作。
// 此DataAsset主要将GameplayTag和输入相关联，方便用于GAS技能系统。
UCLASS()
class BLUR_ARPG_WARRIOR_API UDataAsset_InputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	
	// 默认输入映射。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* DefaultMappingContext;

	// Notes:
	// UPROPERTY宏用来标记UE类中的成员变量，告知引擎如何管理这些成员变量。
	// EditDefaultsOnly：允许变量在蓝图类默认值面板中编辑，但运行时或实例化后的对象不可修改。
	// meta = (TitleProperty = "InputTag")：标头显示属性为InputTag。这会将蓝图编辑器中的抬头显示为 FWarriorInputActionConfig 中 InputTag 成员变量的信息，方便编辑。

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FWarriorInputActionConfig> NativeInputActions;

	// Notes；方法最后的const关键字保证此方法是只读方法，不修改任何变量。

	/// 通过InputTag查找对应的InputAction。
	/// @param InInputTag
	/// @return 
	UInputAction* FindNativeInputActionByTag(const FGameplayTag& InInputTag) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FWarriorInputActionConfig> AbilityInputActions;
};
