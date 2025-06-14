// LJS

#pragma once

#include "CoreMinimal.h"
#include "RPGFunc.h"

class FRPGCustomCountDownAction : public FPendingLatentAction
{
public:
	FRPGCustomCountDownAction(float TotalCountDownTime, float UpdateInterval, float& OutRemainingTime, ERPGCountDownActionOutput& CountDownOutput, const FLatentActionInfo& LatentInfo)

	: bNeedToCancel(false),
	TotalCountDownTime(TotalCountDownTime),
	UpdateInterval(UpdateInterval),
	OutRemainingTime(OutRemainingTime),
	CountDownOutput(CountDownOutput),
	ExecutionFunction(LatentInfo.ExecutionFunction),
	OutputLink(LatentInfo.Linkage),
	CallbackTarget(LatentInfo.CallbackTarget),
	ElapsedInterval(0.f),
	ElapsedTimeSinceStart(0.f)
	{
	}

	virtual void UpdateOperation(FLatentResponse& Response) override;
	
	void CancelAction();
private:
	bool bNeedToCancel;
	
	float TotalCountDownTime;

	float UpdateInterval;

	float& OutRemainingTime;

	ERPGCountDownActionOutput& CountDownOutput;
	
	FName ExecutionFunction;

	int32 OutputLink;

	FWeakObjectPtr CallbackTarget;

	float ElapsedInterval;

	float ElapsedTimeSinceStart;
	
};