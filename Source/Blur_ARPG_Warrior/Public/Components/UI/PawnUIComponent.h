// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponentBase.h"
#include "PawnUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPercentChangedDelegate, float, NewPercent);

//人物UI组件基类，用于和UI通信。
UCLASS()
class BLUR_ARPG_WARRIOR_API UPawnUIComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:

	//Notes: BlueprintAssignable 蓝图可分配
	// BlueprintAssignable 使得蓝图可以分配和使用此委托。因为 UI 的通信收发大多数在蓝图中操作。
	
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentHealthChanged;
};
