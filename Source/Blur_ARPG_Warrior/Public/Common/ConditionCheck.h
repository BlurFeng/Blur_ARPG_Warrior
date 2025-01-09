// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ConditionCheck.generated.h"

//通用条件确认类。
UCLASS()
class BLUR_ARPG_WARRIOR_API UConditionCheck : public UObject
{
	GENERATED_BODY()

public:
	UConditionCheck();

public:

	//Notes：BlueprintNativeEvent 同时允许C++和蓝图来重写的方法。
	//当我们想构建一个同时允许C++和蓝图重写的方法时，使用 UFUNCTION(BlueprintNativeEvent) 说明符。
	//并同时多创建一个同名的带有 _Implementation 尾缀的方法。
	//C++可以重写 _Implementation 方法。而蓝图重写 原方法。在C++中调用时调用 原方法，而不是 _Implementation 方法。
	
	//条件确认方法。允许蓝图覆盖的方法。
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Condition Check")
	bool CheckCondition();

	// 条件确认方法。C++中实现的方法。
	virtual bool CheckCondition_Implementation();
};