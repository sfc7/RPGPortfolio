// LJS


#include "Quest/RPGQuestSystemActor.h"

#include "Character/Player/PlayerCharacterBase.h"

void ARPGQuestSystemActor::BeginPlay()
{
	Super::BeginPlay();

	GetQuestDetails();

	if (APlayerCharacterBase* Player = Cast<APlayerCharacterBase>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		Player->OnInteractQuest.AddDynamic(this, &ARPGQuestSystemActor::OnObjectiveIDHeard);
	}

	IsComplete = AreObjectivesComplete();
}

ARPGQuestSystemActor::ARPGQuestSystemActor()
{
}

void ARPGQuestSystemActor::SetQuestID(FName QuestIDtoSet)
{
	QuestID = QuestIDtoSet;
}

void ARPGQuestSystemActor::SetCurrentStage(int32 QuestStageToSet)
{
	CurrentStage = QuestStageToSet;
}

void ARPGQuestSystemActor::OnObjectiveIDHeard(FString ObjectiveID)
{
	if (!CurrentObjectiveProgress.Contains(ObjectiveID))
	{
		return;
	}

	const int32 CurrentProgress = *CurrentObjectiveProgress.Find(ObjectiveID);
	const FObjectiveDetail ObjectiveDetail = GetObjectiveDataByID(ObjectiveID);
	const int32 NewProgress = FMath::Clamp(CurrentProgress + 1, 0, ObjectiveDetail.Quantity);
	
	CurrentObjectiveProgress.Add(ObjectiveID, NewProgress);
	IsObjectiveComplete(ObjectiveID);
	OnObjectiveHeard.Broadcast();
}

void ARPGQuestSystemActor::GetQuestDetails()
{
	QuestDetails = GetGameInstance()->GetSubsystem<UQuestManager>()->GetQuestFromDataTable(QuestID);
	if (QuestDetails.QuestStages.IsValidIndex(CurrentStage))
	{
		CurrentStageDetails = QuestDetails.QuestStages[CurrentStage];
		CurrentObjectiveProgress.Empty();

		for (FObjectiveDetail ObjectiveDetail : CurrentStageDetails.Objectives)
		{
			CurrentObjectiveProgress.Add(ObjectiveDetail.ObjectiveID, 0);
		}
	}
}

FObjectiveDetail ARPGQuestSystemActor::GetObjectiveDataByID(FString ObjectiveID)
{
	for (FObjectiveDetail ObjectiveDetail : CurrentStageDetails.Objectives)
	{
		if (ObjectiveDetail.ObjectiveID == ObjectiveID)
		{
			return ObjectiveDetail;
		}
	}

	return FObjectiveDetail();
}

void ARPGQuestSystemActor::SetCurrentObjectiveProgress(TMap<FString, int32> ObjectiveProgressToSet)
{
	CurrentObjectiveProgress = ObjectiveProgressToSet;
}

void ARPGQuestSystemActor::IsObjectiveComplete(FString ObjectiveID)
{
	FObjectiveDetail ObjectiveDetail = GetObjectiveDataByID(ObjectiveID);
	if (*CurrentObjectiveProgress.Find(ObjectiveID) >= ObjectiveDetail.Quantity)
	{
		IsComplete = AreObjectivesComplete();
	}
	
}

bool ARPGQuestSystemActor::AreObjectivesComplete()
{
	for (FObjectiveDetail ObjectiveDetail : CurrentStageDetails.Objectives)
	{
		if (*CurrentObjectiveProgress.Find(ObjectiveDetail.ObjectiveID) >= GetObjectiveDataByID(ObjectiveDetail.ObjectiveID).Quantity)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return true;
}




