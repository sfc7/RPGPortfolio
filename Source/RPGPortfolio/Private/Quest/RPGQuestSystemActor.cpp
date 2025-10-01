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

	// 초기 완료 상태 확인
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
	// 목표 ID가 현재 진행 중인 목표에 포함되지 않으면 리턴
	if (!CurrentObjectiveProgress.Contains(ObjectiveID)) return;

	// 현재 진행도 가져오기
	const int32 CurrentProgress = *CurrentObjectiveProgress.Find(ObjectiveID);
	const FObjectiveDetail ObjectiveDetail = GetObjectiveDataByID(ObjectiveID);
	const int32 NewProgress = FMath::Clamp(CurrentProgress + 1, 0, ObjectiveDetail.Quantity);

	// 진행도 업데이트
	CurrentObjectiveProgress.Add(ObjectiveID, NewProgress);
	IsObjectiveComplete(ObjectiveID);
	OnObjectiveHeard.Broadcast();
}

void ARPGQuestSystemActor::GetQuestDetails()
{
	// 퀘스트 매니저에서 퀘스트 데이터 로드
	QuestDetails = GetGameInstance()->GetSubsystem<UQuestManager>()->GetQuestFromDataTable(QuestID);
	
	// 현재 단계가 유효한 인덱스인지 확인	
	if (QuestDetails.QuestStages.IsValidIndex(CurrentStage))
	{
		CurrentStageDetails = QuestDetails.QuestStages[CurrentStage];
		CurrentObjectiveProgress.Empty();

		// 현재 단계의 모든 퀘스트 오브젝트에 대해 진행도 초기화
		for (FObjectiveDetail ObjectiveDetail : CurrentStageDetails.Objectives)
		{
			CurrentObjectiveProgress.Add(ObjectiveDetail.ObjectiveID, 0);
		}
	}
}

FObjectiveDetail ARPGQuestSystemActor::GetObjectiveDataByID(FString ObjectiveID)
{
	// 현재 단계의 퀘스트 오브젝트들 중에서 해당 ID 찾기
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
	// 진행도가 목표 값에 도달했는지 확인
	if (*CurrentObjectiveProgress.Find(ObjectiveID) >= ObjectiveDetail.Quantity)
	{
		IsComplete = AreObjectivesComplete();
	}
	
}

bool ARPGQuestSystemActor::AreObjectivesComplete()
{
	// 모든 오브젝트 진행도 확인후 완료되었는지 확인
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




