// LJS


#include "Animation/PlayerCharacterAnimInstance.h"
#include "Character/Player/PlayerCharacterBase.h"

void UPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (OwningCharacter)
	{
		OwningPlayerCharacter = Cast<APlayerCharacterBase>(OwningCharacter);
	}
}

void UPlayerCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	// 이동 중이면 Idle 시간 초기화
	if (bHasAcceleration)
	{
		IdleElpasedTime = 0.f;
		bShouldEnterRelaxState = false;
	}
	// 멈춰 있으면 Idle 시간 누적
	else
	{
		IdleElpasedTime += DeltaSeconds;
 
		bShouldEnterRelaxState = (IdleElpasedTime >= EnterRelaxtStateTime);
	}
}
