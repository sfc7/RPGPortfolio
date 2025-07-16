// LJS

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstanceBase.h"
#include "CharacterAnimInstance.generated.h"

class ARPGCharacterBase;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UCharacterAnimInstance : public UAnimInstanceBase
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);

	UFUNCTION(BlueprintCallable)
	void SetbShowWeapon(bool flag);

	UFUNCTION(BlueprintCallable)
	void SetbFirstUnEquipWeapon(bool flag);
protected:
	UPROPERTY()
	ARPGCharacterBase* OwningCharacter;
 
	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent;
 
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;
 
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float LocomotionDirection;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AnimData|LocomotionData")
	bool bShowWeapon = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AnimData|LocomotionData")
	bool bFirstUnEquipWeapon = false;
};
