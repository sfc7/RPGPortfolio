// LJS


#include "Func/RPGCustomCountDownAction.h"

#include "RPGStructTypes.h"

void FRPGCustomCountDownAction::UpdateOperation(FLatentResponse& Response)
{
	// 취소 요청 확인
	if (bNeedToCancel)
	{
		CountDownOutput = ERPGCountDownActionOutput::Cancelled;
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}

	// 카운트다운 완료 확인
	const bool bIsCountDownComplete = (ElapsedTimeSinceStart >= TotalCountDownTime);
	if (bIsCountDownComplete)
	{
		CountDownOutput = ERPGCountDownActionOutput::Completed;
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}

	// 업데이트 간격 체크
	const bool bShouldUpdate = (ElapsedInterval >= UpdateInterval);
	if (!bShouldUpdate)
	{
		// 누적시간 계산
		ElapsedInterval += Response.ElapsedTime();
		return;
	}

	// 업데이트 간격에 도달하면 시간 업데이트 및 콜백 트리거 로직
	const float TimeIncrement = UpdateInterval > 0.f ? UpdateInterval : Response.ElapsedTime();
	ElapsedTimeSinceStart += TimeIncrement;

	// 남은 시간 계산 및 출력
	OutRemainingTime = TotalCountDownTime - ElapsedTimeSinceStart;

	// 업데이트 상태로 설정
	CountDownOutput = ERPGCountDownActionOutput::Updated;

	// 블루프린트 콜백 트리거
	Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);

	// 누적시간 리셋
	ElapsedInterval = 0.f;
}

void FRPGCustomCountDownAction::CancelAction()
{
	bNeedToCancel = true;
}