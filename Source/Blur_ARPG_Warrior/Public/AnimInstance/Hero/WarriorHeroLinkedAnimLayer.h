// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance/WarriorBaseAnimInstance.h"
#include "WarriorHeroLinkedAnimLayer.generated.h"

class UWarriorHeroAnimInstance;
//链接动画层基类
UCLASS()
class BLUR_ARPG_WARRIOR_API UWarriorHeroLinkedAnimLayer : public UWarriorBaseAnimInstance
{
	GENERATED_BODY()

public:

	//Notes：BlueprintThreadSafe 特性使此方法是线程安全的。
	//UE5新的动画蓝图将逻辑更新从 主线程 转移到了 其他线程。以提高动画蓝图性能。
	//不是 BlueprintThreadSafe 的方法无法在动画蓝图逻辑流程中使用。
	
	/// 获取英雄动画实例
	/// @return 
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	UWarriorHeroAnimInstance* GetHeroAnimInstance() const;
};
