// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameMode/GameManager/QuestManager.h"
#include "RPGStructTypes.h"
#include "GameAbilitySystem/RPGAttributeSet.h"
#include "RPGSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	URPGSaveGame();

	UFUNCTION(BlueprintPure)
	TArray<FName> GetCurrentActiveQuests() const { return CurrentActiveQuests; }
    
	UFUNCTION(BlueprintPure)
	TArray<FName> GetCompletedQuests() const { return CompletedQuests; }
    
	UFUNCTION(BlueprintPure)
	TMap<FName, FQuestSaveData> GetQuestProgress() const { return QuestProgress; }

	UFUNCTION(BlueprintPure)
	TArray<FInventorySlot> GetPlayerInventorySlots() const { return PlayerInventorySlots; }

	UFUNCTION(BlueprintPure)
	TArray<FInventorySlot> GetPlayerPotionSlots() const { return PlayerPotionSlots; }

	UFUNCTION(BlueprintPure)
	int32 GetPlayerGold() const { return PlayerGold; }
	
	UFUNCTION(BlueprintCallable)
	void SaveQuestDetails(ARPGQuestSystemActor* Quest);

	UFUNCTION(BlueprintCallable)
	void SetCurrentActiveQuests(TArray<FName> CurrentActiveQuestsToSet);

	UFUNCTION(BlueprintCallable)
	void SetCompletedQuests(TArray<FName> CompletedQuestsToSet);

	UFUNCTION(BlueprintCallable)
	void SetPlayerInventorySlots(TArray<FInventorySlot> PlayerInventorySlotsToSet);

	UFUNCTION(BlueprintCallable)
	void SetPlayerPotionSlots(TArray<FInventorySlot> PlayerPotionSlotsToSet);

	UFUNCTION(BlueprintCallable)
	void SetPlayerGold(int32 Gold);

	UFUNCTION(BlueprintPure)
	TArray<FAttributeSaveData> GetSavedAttributes() const { return SavedAttributes; }

	UFUNCTION(BlueprintCallable)
	void SetSavedAttributes(const TArray<FAttributeSaveData>& Attributes) { SavedAttributes = Attributes; }
	
	UPROPERTY() 
	TArray<FName> CurrentActiveQuests;

	UPROPERTY() 
	TArray<FName> CompletedQuests;

	UPROPERTY() 
	TMap<FName, FQuestSaveData> QuestProgress;

	UPROPERTY()
	TArray<FInventorySlot> PlayerInventorySlots;

	UPROPERTY()
	TArray<FInventorySlot> PlayerPotionSlots;

	UPROPERTY()
	int32 PlayerGold;

	UPROPERTY()
	TArray<FAttributeSaveData> SavedAttributes;
};
