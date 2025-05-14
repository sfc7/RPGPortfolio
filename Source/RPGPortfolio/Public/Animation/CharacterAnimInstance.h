ㅁ// LJS

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

protected:
	UPROPERTY()
	ARPGCharacterBase* OwningCharacter;
 
	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent;
 
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;
 
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;
	
};
