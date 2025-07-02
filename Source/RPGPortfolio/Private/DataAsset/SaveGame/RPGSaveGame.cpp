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

void URPGSaveGame::SaveQuestLog()
{
	CurrentActiveQuests = GetWorld()->GetGameInstance()->GetSubsystem<UQuestManager>()->GetCurrentActiveQuests();
	CompletedQuests = GetWorld()->GetGameInstance()->GetSubsystem<UQuestManager>()->GetCompletedQuests();
	
}

void URPGSaveGame::LoadQuests()
{
	for (FName CurrentActiveQuest : CurrentActiveQuests)
	{
		ARPGQuestSystemActor* AddQuest = GetWorld()->GetGameInstance()->GetSubsystem<UQuestManager>()->AddNewQuest(CurrentActiveQuest);
		FQuestSaveData* FindQuestSaveData = QuestProgress.Find(CurrentActiveQuest);
		AddQuest->SetCurrentStage(FindQuestSaveData->CurrentStage);
		AddQuest->SetCurrentObjectiveProgress(FindQuestSaveData->QuestProgress);
	}

	GetWorld()->GetGameInstance()->GetSubsystem<UQuestManager>()->SetCurrentActiveQuests(CurrentActiveQuests);
	GetWorld()->GetGameInstance()->GetSubsystem<UQuestManager>()->SetCompleteActiveQuests(CompletedQuests);
}
