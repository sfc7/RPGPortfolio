// LJS

#pragma once

#include "CoreMinimal.h"
#include "Character/RPGCharacterBase.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UDataAsset_InputConfig;
struct FInputActionValue;
class UPlayerCombatComponent;

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API APlayerCharacter : public ARPGCharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	FORCEINLINE UPlayerCombatComponent* GetHeroCombatComponent() const { return PlayerCombatComponent; }
protected:
	virtual void PossessedBy(AController* NewController) override;
	
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UPlayerCombatComponent* PlayerCombatComponent;
			
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
		UDataAsset_InputConfig* InputConfigDataAsset;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

};
