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
 
	if (bHasAcceleration)
	{
		IdleElpasedTime = 0.f;
		bShouldEnterRelaxState = false;
	}
	else
	{
		IdleElpasedTime += DeltaSeconds;
 
		bShouldEnterRelaxState = (IdleElpasedTime >= EnterRelaxtStateTime);
	}
}
