// LJS

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "RPGAT_Player_RotateTarget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRotateTargetTickDelegate,float,DeltaTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRotationCompletedDelegate); 
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGAT_Player_RotateTarget : public UAbilityTask
{
	GENERATED_BODY()
public:
	URPGAT_Player_RotateTarget();
	
	virtual void TickTask(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility",DefaultToSelf = "OwningAbility",BlueprintInternalUseOnly = "true"))
	static URPGAT_Player_RotateTarget* ExecuteTaskOnTick(UGameplayAbility* OwningAbility);
	
	UPROPERTY(BlueprintAssignable)
	FOnRotationCompletedDelegate OnRotationCompleted;

	UPROPERTY(BlueprintAssignable)
	FOnRotateTargetTickDelegate OnRotateTargetTaskTick;
	
	FRotator TargetRotation;
	bool bIsRotating = false;
	float RotationTolerance = 5.0f;
	
	UFUNCTION(BlueprintCallable)
	void SetTargetRotation(FRotator NewTargetRotation);

	UFUNCTION()
	void CheckRotationComplete(float DeltaTime);
};
