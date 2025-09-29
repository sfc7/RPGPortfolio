// LJS

#pragma once

#include "CoreMinimal.h"
#include "Character/RPGCharacterBase.h"
#include "RPGStructTypes.h"
#include "GameplayTagContainer.h"
#include "PlayerCharacterBase.generated.h"

class UPlayerInventoryComponent;
class UInventoryComponent;
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
class UPlayerEquipmentComponent;

// 퀘스트 상호작용 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractQuest, FString, ObjectiveID);

/**
 * ARPGCharacterBase
 * 
 * 플레이어 Character의 Base 클래스 
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
	
	virtual UInventoryComponent* GetPlayerPotionHotBar() const;
	
	virtual UObjectPoolComponent* GetObjectPoolComponent() const;
	
	virtual UPlayerSkillComponent* GetPlayerSkillComponent() const;
	
	virtual UPlayerEquipmentComponent* GetPlayerEquipmentComponent() const;

	// 퀘스트 상호작용 델리게이트
	FOnInteractQuest OnInteractQuest;
	
	UFUNCTION()
	virtual void OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//@ GAS 어빌리티 관련 SaveGame용 함수
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void SaveAllPlayerData(URPGSaveGame* SaveGame);

	//@ GAS 어빌리티 관련 SaveGame용 함수
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void LoadAllPlayerData(URPGSaveGame* SaveGame);
	
protected:
	virtual void PossessedBy(AController* NewController) override;
	
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void CreateDefaultAttributeSet() override;

	virtual void Tick(float DeltaSeconds) override;

	//@ 추후 캐릭터 클래스용 열거형
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

	//@ GAS 형태로 처리할 플레이어 InputAction관련 DataAsset
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
		UDataAsset_InputConfig* InputConfigDataAsset;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UPlayerUIComponent* PlayerUIComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UPlayerInventoryComponent* PlayerItemInventoryComponent;

	//@ 포션 퀵슬롯용 인벤토리
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UInventoryComponent* PlayerPotionHotbar;

	//@ 충돌체 스킬용 오브젝트 풀
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UObjectPoolComponent* ObjectPoolComponent;

	//@ 스킬을 GameAbility단위로 관리하는 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UPlayerSkillComponent* PlayerSkillComponent;

	//@ 장비용 인벤토리
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UPlayerEquipmentComponent* PlayerEquipmentComponent;

	//@ 상호작용 대상 및 관련 데이터 캐싱
	FInteractionData InteractionTargetData;

	//@ 상호작용 매니저 캐싱
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInteractManager* InteractManager;

	//@ 락온 대상 변경 벡터
	FVector2D SwitchDirection = FVector2D::ZeroVector;

	//@ 빈도마다 사용할 상호작용 함수
	void PerformInteractionCheck();
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue);
	void Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue);
	void Input_AbilityInputPressed(FGameplayTag _InputTag);
	void Input_AbilityInputReleased(FGameplayTag _InputTag);
	void Input_ShowDebug();
	void Input_CallPauseMenu();
	void Input_CallInventoryUI();
	void Input_CallQuestUI();
	void Input_CallSkillUI();
	void Input_Interact();

};
