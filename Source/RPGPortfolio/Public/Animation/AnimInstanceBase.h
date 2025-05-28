// LJS

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "AnimInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	bool DoesOwnerHaveTag(FGameplayTag TagToCheck) const;
}; 
