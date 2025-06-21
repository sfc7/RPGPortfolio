// LJS

#pragma once

#include "CoreMinimal.h"
#include "Character/RPGCharacterBase.h"
#include "RPGStructTypes.h"
#include "GameplayTagContainer.h"
#include "PlayerCharacterBase.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UDataAsset_InputConfig;
struct FInputActionValue;
class UPlayerCombatComponent;
class UPlayerUIComponent;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API APlayerCharacterBase : public ARPGCharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacterBase();

	FORCEINLINE UPlayerCombatComponent* GetPlayerCombatComponent() const { return PlayerCombatComponent; }

	UInputMappingContext* GetParryingInputMappingContext() const { return ParryingInputMappingContext; }
	
	virtual UCombatComponentBase* GetCombatComponent() const override;

	virtual UUIComponentBase* GetUIComponent() const override;

	virtual UPlayerUIComponent* GetPlayerUIComponent() const override;
	
protected:
	virtual void PossessedBy(AController* NewController) override;
	
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void CreateDefaultAttributeSet() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Class", meta = (AllowPrivateAccess = "true"))
	EPlayerCharacterClass PlayerCharacterClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* ParryingInputMappingContext;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UPlayerCombatComponent* PlayerCombatComponent;
			
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
		UDataAsset_InputConfig* InputConfigDataAsset;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UPlayerUIComponent* PlayerUIComponent;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_AbilityInputPressed(FGameplayTag _InputTag);
	void Input_AbilityInputReleased(FGameplayTag _InputTag);
	void Input_ShowDebug();
	void Input_CallPauseMenu();
	

};
