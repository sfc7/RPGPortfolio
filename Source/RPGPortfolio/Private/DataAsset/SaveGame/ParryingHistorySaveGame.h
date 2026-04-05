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

	UPROPERTY(VisibleAnywhere)
	FString SaveSlotName = TEXT("ParryingSuccessData");

	//@ Save할 유저 인덱스
	UPROPERTY(VisibleAnywhere)
	uint32 UserIndex = 0;
};
