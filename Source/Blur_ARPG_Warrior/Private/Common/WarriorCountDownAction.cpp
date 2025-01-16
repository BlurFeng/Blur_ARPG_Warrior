// Blur Feng All Rights Reserved.


#include "Common/WarriorCountDownAction.h"

#include "Kismet/GameplayStatics.h"

void FWarriorCountDownAction::UpdateOperation(FLatentResponse& Response)
{
	//FPendingLatentAction::UpdateOperation(Response);

	// 游戏暂停时不更新。
	if (UGameplayStatics::IsGamePaused(WorldContextObject) && PausedWithGame)
		return;

	//取消。
	if (bNeedToCancel)
	{
		CountDownOutput = EWarriorCountDownActionOutput::Cancelled;
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}

	//完成。
	if (TotalCountDownTime > 0 && TimerFromStart >= TotalCountDownTime)
	{
		CountDownOutput = EWarriorCountDownActionOutput::Completed;
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}

	//计时器增加。
	TimerFromStart +=  Response.ElapsedTime();
	IntervalTimer += Response.ElapsedTime();
	bool Update = false;

	//确认是否进行Update执行引脚。
	if (UpdateInterval > 0)
	{
		if (IntervalTimer >= UpdateInterval)
		{
			IntervalTimer -= UpdateInterval;
			Update = true;
		}
		
		//在开始的第一帧执行。当没有 UpdateInterval 间隔时间时不用判断。
		if (ExecuteOnFirst == true)
		{
			ExecuteOnFirst = false;
			OutRemainingTime = TotalCountDownTime;
			CountDownOutput = EWarriorCountDownActionOutput::Update; //输出执行引脚
			Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);
		}
	}
	else
	{
		Update = true;
	}

	//更新
	if (Update)
	{
		OutRemainingTime = TotalCountDownTime - TimerFromStart;
		OutRemainingTime = OutRemainingTime > 0.f ? OutRemainingTime : 0.f;
		CountDownOutput = EWarriorCountDownActionOutput::Update; //输出执行引脚
		Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);
	}
}

void FWarriorCountDownAction::CancelAction()
{
	bNeedToCancel = true;
}
