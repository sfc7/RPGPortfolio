// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ParryingHistorySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class UParryingHistorySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// 현재 성공률
	UPROPERTY(VisibleAnywhere) 
	float SuccessfulRate = 1.f;

	// 누적 성공률
	UPROPERTY(VisibleAnywhere) 
	float TotalSuccessfulRate = 1.f;

	// 현재 기록된 카운트
	UPROPERTY(VisibleAnywhere) 
	int RecordCount = 0;

	// ResetCount회 마다 성공률 측정
	UPROPERTY(VisibleAnywhere) 
	int ResetCount = 3;
	
	UPROPERTY(VisibleAnywhere)
	FString SaveSlotName = TEXT("ParryingSuccessData");
    
	UPROPERTY(VisibleAnywhere)
	uint32 UserIndex = 0;
};
	