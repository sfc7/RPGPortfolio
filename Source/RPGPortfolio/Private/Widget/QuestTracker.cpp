// LJS


#include "Widget/QuestTracker.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Quest/RPGQuestSystemActor.h"
#include "Widget/QuestEntry_Objective.h"
#include "Widget/RPGQuestLog_QuestEntry.h"

void UQuestTracker::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}
	
void UQuestTracker::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UQuestTracker::UpdateQuestActor(ARPGQuestSystemActor* QuestActorToSet)
{
	QuestActor = QuestActorToSet;
	ObjectiveBox->ClearChildren();

	// 퀘스트 액터를 이벤트에 바인딩
	QuestActor->OnObjectiveHeard.AddDynamic(this, &ThisClass::OnObjectiveHeard);

	SetQuestActor(QuestActor);
}

void UQuestTracker::SetQuestActor(ARPGQuestSystemActor* QuestActorToSet)
{
	if (!IsValid(QuestActorToSet)) return;
	
	if (!IsValid(QuestName) || !IsValid(ObjectiveBox)) return;
	
	QuestActor = QuestActorToSet;
    
	FQuest QuestDetails = QuestActor->GetQuestDetailVariable();
	if (QuestDetails.QuestStages.Num() == 0)
	{
		return;
	}
    
	QuestName->SetText(QuestDetails.QuestName);

	// 퀘스트의 첫번째 스테이지가 있는지로 유효성을 체크하여 오브젝트들 가져와서 위젯 생성
	if (QuestDetails.QuestStages.IsValidIndex(0))
	{
		const TArray<FObjectiveDetail>& Objectives = QuestDetails.QuestStages[0].Objectives;
		for (const FObjectiveDetail& ObjectiveDetail : Objectives)
		{            
			UUserWidget* Widget = CreateWidget(GetWorld(), QuestEntry_ObjectiveWidgetClass);
			UQuestEntry_Objective* ObjectiveWidget = Cast<UQuestEntry_Objective>(Widget);
			if (ObjectiveWidget)
			{
				ObjectiveWidget->SetObjectiveData(ObjectiveDetail, QuestActor);
				ObjectiveBox->AddChildToVerticalBox(ObjectiveWidget);
			} 
		}
	}

	// 퀘스트 액터를 이벤트에 바인딩
	QuestActor->OnObjectiveHeard.AddDynamic(this, &ThisClass::OnObjectiveHeard);
}

void UQuestTracker::QuestCompleted(ARPGQuestSystemActor* QuestActorToSet)
{
	if (QuestActorToSet == QuestActor)
	{
		RemoveFromParent();
	}
}

void UQuestTracker::OnObjectiveHeard()
{
	UpdateQuestActor(QuestActor);
}
