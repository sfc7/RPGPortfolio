// LJS


#include "Animation/CharacterAnimInstance.h"
#include "Character/RPGCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	OwningCharacter = Cast<ARPGCharacterBase>(TryGetPawnOwner());
 
	if (IsValid(OwningCharacter))
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (!IsValid(OwningCharacter) || !IsValid(OwningMovementComponent)) return;
 
	GroundSpeed = OwningCharacter->GetVelocity().Size2D();
 
	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D()>0.f;

	LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(OwningCharacter->GetVelocity(), OwningCharacter->GetActorRotation());
}
