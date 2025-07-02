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

	UFUNCTION(BlueprintCallable)
	void SaveQuestDetails(ARPGQuestSystemActor* Quest);

	UFUNCTION(BlueprintCallable)
	void SaveQuestLog();

	UFUNCTION(BlueprintCallable)
	void LoadQuests();
	
private:
	TArray<FName> CurrentActiveQuests;

	TArray<FName> CompletedQuests;

	TMap<FName, FQuestSaveData> QuestProgress;
};
