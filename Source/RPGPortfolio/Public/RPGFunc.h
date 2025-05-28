// LJS

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGFunc.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGFunc : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static bool CheckTargetTeamAgent(APawn* SourcePawn, APawn* TargetPawn);
};
