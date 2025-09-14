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
	
	UPROPERTY()
	float BaseCanParryingStateDelayTime = 0.5f;

	UPROPERTY()
	float AddCanParryingStateDelayTime = 0.25f;
	
	UPROPERTY()
	float PreviousSessionAlpha = 1.0f;
};