// LJS


#include "Func/RPGCustomCountDownAction.h"

#include "RPGStructTypes.h"

void FRPGCustomCountDownAction::UpdateOperation(FLatentResponse& Response)
{
	if (bNeedToCancel)
	{
		CountDownOutput = ERPGCountDownActionOutput::Cancelled;

		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);

		return;
	}

	if (ElapsedTimeSinceStart >= TotalCountDownTime)
	{
		CountDownOutput = ERPGCountDownActionOutput::Completed;

		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		
		return;
	}

	if (ElapsedInterval < UpdateInterval)
	{
		ElapsedInterval += Response.ElapsedTime();
	}
	else
	{
		ElapsedTimeSinceStart += UpdateInterval > 0.f? UpdateInterval : Response.ElapsedTime();

		OutRemainingTime = TotalCountDownTime - ElapsedTimeSinceStart;

		CountDownOutput = ERPGCountDownActionOutput::Updated;

		Response.TriggerLink(ExecutionFunction, OutputLink,CallbackTarget);

		ElapsedInterval = 0.f;
	}
}

void FRPGCustomCountDownAction::CancelAction()
{
	bNeedToCancel = true;
}
