// LJS

#pragma once

#include "CoreMinimal.h"
#include "Animation/CharacterAnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

class APlayerCharacterBase;

/**
 * UPlayerCharacterAnimInstance
 * 
 * PlayerCharacter를 위한 AnimInstance
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
	APlayerCharacterBase* OwningPlayerCharacter;

	// 휴식 상태로 전환할지 여부
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bShouldEnterRelaxState;

	// 휴식 상태 전환까지 걸리는 시간
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float EnterRelaxtStateTime = 5.f;

	// Idle 상태에서 경과한 시간
	float IdleElpasedTime;
	
};
