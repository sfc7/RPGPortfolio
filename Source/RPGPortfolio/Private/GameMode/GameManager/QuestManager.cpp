// LJS


#include "GameMode/GameManager/QuestManager.h"

#include "Quest/RPGQuestSystemActor.h"

void UQuestManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	QuestData = LoadObject<UDataTable>(nullptr, TEXT("/Game/MyProject/Data/Quest/DT_QuestTable.DT_QuestTable"));
}

void UQuestManager::AddNewQuest(FName QuestID)
{
	CurrentActiveQuests.AddUnique(QuestID);

	ARPGQuestSystemActor* QuestActor = GetWorld()->SpawnActorDeferred<ARPGQuestSystemActor>(
		ARPGQuestSystemActor::StaticClass(),
		FTransform(FVector::ZeroVector), 
		nullptr,
		nullptr
	);

	if (QuestActor)
	{
		QuestActor->SetQuestID(FName("YourQuestID"));
		QuestActor->FinishSpawning(FTransform(FVector::ZeroVector));
	}

	CurrentQuests.Add(QuestActor);
}

void UQuestManager::CompleteQuest()
{
}

bool UQuestManager::QueryActiveQuest(FName QuestID)
{
	if (CurrentActiveQuests.Contains(QuestID)) return true;

	return false;
}

void UQuestManager::TrackQuest()
{
}


	