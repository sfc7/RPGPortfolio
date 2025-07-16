// LJS

#pragma once

#include "CoreMinimal.h"
#include "Character/RPGCharacterBase.h"
#include "RPGStructTypes.h"
#include "GameplayTagContainer.h"
#include "PlayerCharacterBase.generated.h"

class UPlayerInventoryComponent;
class UCameraComponent;
class USpringArmComponent;
class UDataAsset_InputConfig;
struct FInputActionValue;
class UPlayerCombatComponent;
class UPlayerUIComponent;
class UInputMappingContext;
struct FInteractionData;
class UInteractManager;
class UObjectPoolComponent;
class UPlayerSkillComponent;
class URPGSaveGame;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractQuest, FString, ObjectiveID);
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

	virtual UCameraComponent* GetCameraComponent() const;

	virtual UPlayerUIComponent* GetPlayerUIComponent() const override;

	virtual UPlayerInventoryComponent* GetPlayerInventoryComponent() const;

	virtual UPlayerInventoryComponent* GetPlayerPotionHotBar() const;

	virtual UObjectPoolComponent* GetObjectPoolComponent() const;

	virtual UPlayerSkillComponent* GetPlayerSkillComponent() const;

	FOnInteractQuest OnInteractQuest;
	
	UFUNCTION()
	virtual void OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void SaveAllPlayerData(URPGSaveGame* SaveGame);

	UFUNCTION(BlueprintCallable, Category = "Save System")
	
	void LoadAllPlayerData(URPGSaveGame* SaveGame);
protected:
	virtual void PossessedBy(AController* NewController) override;
	
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void CreateDefaultAttributeSet() override;

	virtual void Tick(float DeltaSeconds) override;
	
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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UPlayerInventoryComponent* PlayerItemInventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UPlayerInventoryComponent* PlayerPotionHotbar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UObjectPoolComponent* ObjectPoolComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UPlayerSkillComponent* PlayerSkillComponent;

	FInteractionData InteractionTargetData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInteractManager* InteractManager;

	void PerformInteractionCheck();
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_AbilityInputPressed(FGameplayTag _InputTag);
	void Input_AbilityInputReleased(FGameplayTag _InputTag);
	void Input_ShowDebug();
	void Input_CallPauseMenu();
	void Input_CallInventoryUI();
	void Input_CallQuestUI();
	void Input_CallSkillUI();
	void Input_Interact();

};
