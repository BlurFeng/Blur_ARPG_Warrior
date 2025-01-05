// Blur Feng All Rights Reserved.


#include "WarriorTypes/WarriorCountDownAction.h"

void FWarriorCountDownAction::UpdateOperation(FLatentResponse& Response)
{
	//FPendingLatentAction::UpdateOperation(Response);

	//取消。
	if (bNeedToCancel)
	{
		CountDownOutput = EWarriorCountDownActionOutput::Cancelled;
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}

	//完成。
	if (ElapsedTimeSinceStart >= TotalCountDownTime)
	{
		CountDownOutput = EWarriorCountDownActionOutput::Completed;
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}
	
	//更新。
	if (ElapsedIntervalTimer < UpdateInterval)
	{
		ElapsedIntervalTimer += Response.ElapsedTime();

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
		//Tips：在 Completed 后更新计时，那么在倒计时结束时会调用一次 Update，然后在下一帧调用 Completed。
		
		//增加经过时间。当 UpdateInterval<=0 时，会每帧更新。
		ElapsedTimeSinceStart += UpdateInterval > 0.f ? UpdateInterval : Response.ElapsedTime();

		//触发执行引脚。
		OutRemainingTime = TotalCountDownTime - ElapsedTimeSinceStart; //剩余时间
		OutRemainingTime = OutRemainingTime > 0.f ? OutRemainingTime : 0.f;
		CountDownOutput = EWarriorCountDownActionOutput::Update; //输出执行引脚
		Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);

		//Tips：使用 -=UpdateInterval 来重置ElapsedInterval而不是 =0 。因为后者会产生少量的时间丢失导致问题。
		//当然在UpdateOperation方法更新的足够快时这个问题不容易暴露出来。

		//重置经过时间间隔计时器。
		ElapsedIntervalTimer -= UpdateInterval;
	}
}

void FWarriorCountDownAction::CancelAction()
{
	bNeedToCancel = true;
}
