// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_ExecuteTaskOnTick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityTaskTickDelegate, float, DeltaTime);

//用于在GA中进行Tick更新。
UCLASS()
class BLUR_ARPG_WARRIOR_API UAbilityTask_ExecuteTaskOnTick : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAbilityTask_ExecuteTaskOnTick();

	//Notes：在蓝图中隐藏参数和指定默认参数。
	//HidePin 用于在蓝图隐藏参数节点。
	//DefaultToSelf 用于指定参数的默认值为调用改函数的对象。
	//Notes：限制蓝图方法只在类内部可用。
	//BlueprintInternalUseOnly 表示该变量只能在蓝图类的成员函数中进行读写操作。常用于定义只能在类内部访问的变量。

	//用于在 GameplayAbility 蓝图中执行Tick。
	UFUNCTION(BlueprintCallable, Category = "Warrior|AbilityTasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UAbilityTask_ExecuteTaskOnTick* ExecuteTaskOnTick(UGameplayAbility* OwningAbility);

	//~ Begin UGameplayTask Interface
	virtual void TickTask(float DeltaTime) override;
	//~ End UGameplayTask Interface

	//技能Tick更新时回调。
	UPROPERTY(BlueprintAssignable)
	FOnAbilityTaskTickDelegate OnAbilityTaskTick;
};
