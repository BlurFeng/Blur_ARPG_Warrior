// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WarriorFunctionLibrary.generated.h"

struct FGameplayTag;
class UWarriorAbilitySystemComponent;

UENUM()
enum class EWarriorConfirmType : uint8
{
	Yes,
	No
};

//项目方法库
UCLASS()
class BLUR_ARPG_WARRIOR_API UWarriorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/// 获取AbilitySystemComponent从Actor。
	/// @param InActor 
	/// @return 
	static UWarriorAbilitySystemComponent* NativeGetWarriorASCFromActor(AActor* InActor);

	/// 向Actor（的技能组件）添加一个还未包含的Tag。
	/// @param InActor 目标Actor。
	/// @param TagToAdd 添加Tag。
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	/// 移除一个Tag从Actor。
	/// @param InActor 目标Actor。
	/// @param TagToRemove 移除Tag。
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);

	/// 确认Actor（的技能组件）是否包含Tag。
	/// @param InActor 目标Actor。
	/// @param TagToCheck 确认Tag。
	/// @return 
	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	//Notes：ExpandEnumAsExecs 执行引脚
	//meta = (ExpandEnumAsExecs = "OutConfirmType") 将参数 OutConfirmType 枚举类型转化为蓝图中的执行引脚。免去了Branch判断，便于使用。
	//Notes：DisplayName 显示名称
	//在C++中根据用途，我们可能为某一功能创建一系列的方法，采用不同前缀的命名方式。但在蓝图中，我们不希望显示这些前缀。
	//可以通过 meta = (DisplayName = "Does Actor Have Tag") 来修改显示名称。

	/// 确认Actor是否包含某个Tag。
	/// @param InActor 目标Actor。
	/// @param TagToCheck 确认Tag。
	/// @param OutConfirmType 返回确认结果。
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EWarriorConfirmType& OutConfirmType);
};