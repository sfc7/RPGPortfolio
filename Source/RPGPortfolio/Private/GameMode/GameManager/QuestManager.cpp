// LJS


#include "GameMode/GameManager/QuestManager.h"

#include "Quest/RPGQuestSystemActor.h"

void UQuestManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	QuestDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/MyProject/Data/Quest/DT_QuestTable.DT_QuestTable"));
}

ARPGQuestSystemActor* UQuestManager::AddNewQuest(FName QuestID)
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
		QuestActor->SetQuestID(QuestID);
		QuestActor->FinishSpawning(FTransform(FVector::ZeroVector));
	}

	CurrentQuests.Add(QuestActor);
	return QuestActor;
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

UDataTable* UQuestManager::GetQuestDataTable()
{
	return QuestDataTable;
}

FQuest UQuestManager::GetQuestFromDataTable(FName QuestID)
{
	if (QuestDataTable)
	{
		if (FQuest* QuestData = QuestDataTable->FindRow<FQuest>(QuestID, TEXT("GetQuest")))
		{
			return *QuestData;  // 값 복사해서 반환
		}
	}
	
	return FQuest();  
}

TArray<ARPGQuestSystemActor*> UQuestManager::GetCurrentQuests()
{
	return CurrentQuests;
}

TArray<FName> UQuestManager::GetCurrentActiveQuests()
{
	return CurrentActiveQuests;
}

TArray<FName> UQuestManager::GetCompletedQuests()
{
	return CompletedQuests;
}

void UQuestManager::SetCurrentActiveQuests(TArray<FName> CurrentActiveQuestsToSet)
{
	CurrentActiveQuests = CurrentActiveQuestsToSet;
}

void UQuestManager::SetCompleteActiveQuests(TArray<FName> CompletedQuestsToSet)
{
	CompletedQuests = CompletedQuestsToSet;
}
