// LJS

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstanceBase.h"
#include "CharacterAnimInstance.generated.h"

class ARPGCharacterBase;
class UCharacterMovementComponent;

/**
 * UCharacterAnimInstance
 * 
 * Character를 위한 Base AnimInstance
 */
UCLASS()
class RPGPORTFOLIO_API UCharacterAnimInstance : public UAnimInstanceBase
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);

	// 무기 표시 여부 설정 (무기가 있는 캐릭터를 위함)
	UFUNCTION(BlueprintCallable)
	void SetbShowWeapon(bool flag);

	// 첫 무기 해제 여부 설정
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
