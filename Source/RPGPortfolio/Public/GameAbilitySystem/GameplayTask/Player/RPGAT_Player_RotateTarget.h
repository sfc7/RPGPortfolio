// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GameplayTask/RPGAT_ExecuteTaskOnTick.h"
#include "RPGAT_Player_RotateTarget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRotationCompletedDelegate); 
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGAT_Player_RotateTarget : public UAbilityTask
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable)
	FOnRotationCompletedDelegate OnRotationCompleted;
	
	FRotator TargetRotation;
	bool bIsRotating = false;
	float RotationTolerance = 5.0f;
	
	UFUNCTION(BlueprintCallable)
	void SetTargetRotation(FRotator NewTargetRotation);
	
};
