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
	UPROPERTY(VisibleAnywhere) 
	float SuccessfulRate = 1.f;
	
	UPROPERTY(VisibleAnywhere)
	FString SaveSlotName = TEXT("ParryingSuccessData");
    
	UPROPERTY(VisibleAnywhere)
	uint32 UserIndex = 0;
};
	