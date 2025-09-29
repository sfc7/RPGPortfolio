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

	// 속도 계산 (XY만 사용)
	GroundSpeed = OwningCharacter->GetVelocity().Size2D();
	// 가속 여부 확인
	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;
	// 이동 방향 계산 (Forward/Right 기준)
	LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(OwningCharacter->GetVelocity(), OwningCharacter->GetActorRotation());

}

void UCharacterAnimInstance::SetbShowWeapon(bool flag)
{
	bShowWeapon = flag;
}

void UCharacterAnimInstance::SetbFirstUnEquipWeapon(bool flag)
{
	bFirstUnEquipWeapon = flag;
}
