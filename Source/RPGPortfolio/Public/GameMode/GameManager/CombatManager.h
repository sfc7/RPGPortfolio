// LJS

#pragma once

#include "CoreMinimal.h"
#include "MotionDelayBuffer.h"
#include "Containers/CircularQueue.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CombatManager.generated.h"

class UParryingHistorySaveGame;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UCombatManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	UFUNCTION()
	void RecordParryAttempt(bool bSuccess);
	
	UFUNCTION()
	float GetAdjustedCanParryingStateDelay() const;

	UFUNCTION()
	float GetAdjustedParryingASCActiveTime() const;
	
	UFUNCTION()
	void SaveParryingData();
	
	UFUNCTION()
	void LoadParryingData();

private:
	UPROPERTY()
	UParryingHistorySaveGame* CurrentParryingData;
	
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