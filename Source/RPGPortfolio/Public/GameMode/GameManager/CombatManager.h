// LJS

#pragma once

#include "CoreMinimal.h"
#include "MotionDelayBuffer.h"
#include "Containers/CircularQueue.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CombatManager.generated.h"

class UParryingHistorySaveGame;
/**
 * UCombatManager
 * 
 * 전투 시스템 관리 서브시스템
 */
UCLASS()
class RPGPORTFOLIO_API UCombatManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	//@ 패링 시도 기록 
	UFUNCTION()
	void RecordParryAttempt(bool bSuccess);

	//@ 패링 가능 상태 시간 반환
	UFUNCTION()
	float GetAdjustedCanParryingStateDelay() const;

	// 패링 공격 및 패링 실패 판정에 쓸 시간(패링 판정시간에 * 2) 반환
	UFUNCTION()
	float GetAdjustedParryingASCActiveTime() const;

	//@ 패링 데이터 저장
	UFUNCTION()
	void SaveParryingData();

	//@ 패링 데이터 로드
	UFUNCTION()
	void LoadParryingData();

private:
	//@ 현재 패링 데이터 세이브 게임 인스턴스
	UPROPERTY()
	UParryingHistorySaveGame* CurrentParryingData;

	//@ 최근 패링 결과 순환 큐
	TCircularQueue<bool> TempParryResults{21};
	
	// 패링 타이밍 기본 값
	UPROPERTY()
	float BaseCanParryingStateTime = 0.5f;

	// 최대로 늘어날 수 있는 패링 타이밍
	UPROPERTY()
	float AddCanParryingStateDelayTime = 0.25f;

	// 이전 로그의 성공률을 얼마나 반영할지 Alpha값
	UPROPERTY()
	float SuccessAlpha = 0.5f;
};