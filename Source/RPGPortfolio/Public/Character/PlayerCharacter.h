// LJS

#pragma once

#include "CoreMinimal.h"
#include "Character/RPGCharacterBase.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API APlayerCharacter : public ARPGCharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	
protected:
	
	virtual void BeginPlay() override;

private:

#pragma region component

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* CameraComponent;
#pragma endregion component
	
};
