// LJS

#pragma once

#include "CoreMinimal.h"
#include "RPGStructTypes.h"

/**
 * FRPGCustomCountDownAction
 * 
 * 커스텀 카운트다운 지연 액션 클래스
 * 블루프린트에서 사용가능하게하고 취소 가능하게
 */
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

	//@ 매 틱마다 호출되는 업데이트 함수 - 카운트다운 로직 처리
	virtual void UpdateOperation(FLatentResponse& Response) override;

	//@ 카운트다운 액션 취소
	void CancelAction();
private:
	//@ 취소 요청 플래그
	bool bNeedToCancel;

	//@ 전체 카운트다운 시간
	float TotalCountDownTime;

	// 업데이트 간격
	float UpdateInterval;

	//@ 남은 시간 출력 참조
	float& OutRemainingTime;

	//@ 카운트다운 상태 출력 참조 (완료/업데이트/취소 등등)
	ERPGCountDownActionOutput& CountDownOutput;
	
	//@ 실행할 함수 이름
	FName ExecutionFunction;

	//@ 출력 링크 번호
	int32 OutputLink;

	//@ 콜백 대상 오브젝트 (약한 참조로)
	FWeakObjectPtr CallbackTarget;

	//@ 마지막 업데이트 이후 경과된 간격 시간
	float ElapsedInterval;

	//@ 카운트다운 시작 이후 총 경과 시간
	float ElapsedTimeSinceStart;
	
};