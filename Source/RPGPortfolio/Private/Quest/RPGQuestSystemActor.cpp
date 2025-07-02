// LJS


#include "Quest/RPGQuestSystemActor.h"

#include "Character/Player/PlayerCharacterBase.h"

ARPGQuestSystemActor::ARPGQuestSystemActor()
{
	PrimaryActorTick.bCanEverTick = true;
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
	if (CurrentObjectiveProgress.Find(ObjectiveID))
	{
		int32 Index = *CurrentObjectiveProgress.Find(ObjectiveID);
		FObjectiveDetail CurrentObjectiveDetail = GetObjectiveDataByID(ObjectiveID);
		int AddIndex = FMath::Clamp(Index+1, 0, CurrentObjectiveDetail.Quantity);

		
		CurrentObjectiveProgress.Add(ObjectiveID, AddIndex);

		return;
	}
	else
	{
		return;
	}
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

void ARPGQuestSystemActor::BeginPlay()
{
	Super::BeginPlay();

	GetQuestDetails();

	if (APlayerCharacterBase* Player = Cast<APlayerCharacterBase>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		Player->OnInteractQuest.AddDynamic(this, &ARPGQuestSystemActor::OnObjectiveIDHeard);
	}
}

// Called every frame
void ARPGQuestSystemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

