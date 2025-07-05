// LJS


#include "DataAsset/SaveGame/RPGSaveGame.h"

#include "Quest/RPGQuestSystemActor.h"

URPGSaveGame::URPGSaveGame()
{
}

void URPGSaveGame::SaveQuestDetails(ARPGQuestSystemActor* Quest)
{
	FQuestSaveData SaveData;
	SaveData.QuestProgress = Quest->GetCurrentObjectiveProgress();
	SaveData.CurrentStage = Quest->GetCurrentStage();
	QuestProgress.Add(Quest->GetQuestID(),SaveData);
}

void URPGSaveGame::SetCurrentActiveQuests(TArray<FName> CurrentActiveQuestsToSet)
{
	CurrentActiveQuests = CurrentActiveQuestsToSet;
}

void URPGSaveGame::SetCompletedQuests(TArray<FName> CompletedQuestsToSet)
{
	CompletedQuests = CompletedQuestsToSet;
}

void URPGSaveGame::SetPlayerInventorySlots(TArray<FInventorySlot> PlayerInventorySlotsToSet)
{
	PlayerInventorySlots = PlayerInventorySlotsToSet;
}

void URPGSaveGame::SetPlayerPotionSlots(TArray<FInventorySlot> PlayerPotionSlotsToSet)
{
	PlayerPotionSlots = PlayerPotionSlotsToSet;
}

void URPGSaveGame::SetPlayerGold(int32 Gold)
{
	PlayerGold = Gold;
}
