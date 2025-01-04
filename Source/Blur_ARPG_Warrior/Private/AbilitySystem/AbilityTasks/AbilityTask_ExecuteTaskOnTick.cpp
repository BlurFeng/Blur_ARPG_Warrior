// Blur Feng All Rights Reserved.


#include "AbilitySystem/AbilityTasks/AbilityTask_ExecuteTaskOnTick.h"

UAbilityTask_ExecuteTaskOnTick::UAbilityTask_ExecuteTaskOnTick()
{
	//设置此值为true，TickTask()方法才会执行。
	bTickingTask = true;
}

UAbilityTask_ExecuteTaskOnTick* UAbilityTask_ExecuteTaskOnTick::ExecuteTaskOnTick(UGameplayAbility* OwningAbility)
{
	//创建一个技能任务节点。这是我们必须进行的工作。
	UAbilityTask_ExecuteTaskOnTick* Node = NewAbilityTask<UAbilityTask_ExecuteTaskOnTick>(OwningAbility);
	return Node; 
}

void UAbilityTask_ExecuteTaskOnTick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	//确认可以进行技能任务广播。确认技能任然在激活状态。
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnAbilityTaskTick.Broadcast(DeltaTime);
	}
	//结束任务。
	else
	{
		//在我们认为技能任我结束时，我们应当调用 EndTask() 来结束任务。
		EndTask();
	}
}
