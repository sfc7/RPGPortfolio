// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameMode/GameManager/QuestManager.h"
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
	TMap<FName, FQuestSaveData> GetQuestProgress() { return QuestProgress; }
	
	UFUNCTION(BlueprintCallable)
	void SaveQuestDetails(ARPGQuestSystemActor* Quest);

	UFUNCTION(BlueprintCallable)
	void SetCurrentActiveQuests(TArray<FName> CurrentActiveQuestsToSet);

	UFUNCTION(BlueprintCallable)
	void SetCompletedQuests(TArray<FName> CompletedQuestsToSet);

private:
	UPROPERTY() 
	TArray<FName> CurrentActiveQuests;

	UPROPERTY() 
	TArray<FName> CompletedQuests;

	UPROPERTY() 
	TMap<FName, FQuestSaveData> QuestProgress;
};
