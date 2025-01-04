// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponent.h"
#include "EnemyUIComponent.generated.h"

class UWarriorWidgetBase;

//敌人UI组件，用于管理和清空Widgets。
UCLASS()
class BLUR_ARPG_WARRIOR_API UEnemyUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()

public:
	/// 注册Widget到UI组件。
	/// @param InWidgetToRegister 
	UFUNCTION(BlueprintCallable)
	void RegisterEnemyDrawnWidget(UWarriorWidgetBase* InWidgetToRegister);

	/// 移除所有敌人绘制的UI。
	UFUNCTION(BlueprintCallable)
	void RemoveEnemyDrawnWidgetsIfAny();
	
private:
	TArray<UWarriorWidgetBase*> EnemyDrawnWidgets;
};
