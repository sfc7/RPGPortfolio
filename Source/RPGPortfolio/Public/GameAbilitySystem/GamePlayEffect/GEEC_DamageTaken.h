// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEEC_DamageTaken.generated.h"

/**
 * UGEEC_DamageTaken
 * 
 * 데미지 계산할 GameplayEffect 클래스에 사용할 데미지 관련 계산 
 */
UCLASS()
class RPGPORTFOLIO_API UGEEC_DamageTaken : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UGEEC_DamageTaken();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
};
