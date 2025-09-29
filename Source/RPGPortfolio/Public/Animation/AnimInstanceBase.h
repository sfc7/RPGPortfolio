// LJS

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "AnimInstanceBase.generated.h"

/**
 * UAnimInstanceBase
 * 
 * 모든 AnimInstance의 Base 클래스 
 */
UCLASS()
class RPGPORTFOLIO_API UAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	bool DoesOwnerHaveTag(FGameplayTag TagToCheck) const;
}; 
