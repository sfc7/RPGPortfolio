// LJS

#pragma once

#include "CoreMinimal.h"
#include "Animation/CharacterAnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UPlayerCharacterAnimInstance : public UCharacterAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);
 
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "")
	APlayerCharacter* OwningPlayerCharacter;
 
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bShouldEnterRelaxState;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float EnterRelaxtStateTime = 5.f;
 
	float IdleElpasedTime;
	
};
