// LJS


#include "Widget/QuestLogWidget.h"

#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"
#include "GameMode/GameManager/QuestManager.h"
#include "GameMode/GameManager/UIManager.h"
#include "Quest/RPGQuestSystemActor.h"
#include "Widget/RPGQuestLog_QuestEntry.h"
#include "Widget/QuestEntry_Objective.h"

void UQuestLogWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	TArray<ARPGQuestSystemActor*> CurrentQuests = GetWorld()->GetGameInstance()->GetSubsystem<UQuestManager>()->GetCurrentQuests();
	
	for (ARPGQuestSystemActor* CurrentQuest : CurrentQuests)
	{
		FName CurrentQuestID = CurrentQuest->GetQuestID();
		if (!GetWorld()->GetGameInstance()->GetSubsystem<UQuestManager>()->QueryActiveQuest(CurrentQuestID)) continue;
		
		FQuest QuestDetail =GetWorld()->GetGameInstance()->GetSubsystem<UQuestManager>()->GetQuestFromDataTable(CurrentQuestID);
		UUserWidget* Widget = CreateWidget(GetWorld(), Quest_LogEntryWidgetClass);
		URPGQuestLog_QuestEntry* QuestEntryWidget = Cast<URPGQuestLog_QuestEntry>(Widget);
		if (QuestEntryWidget)
		{
			QuestEntryWidget->SetQuestID(CurrentQuestID);
			QuestEntryWidget->SetQuestActor(CurrentQuest);
			QuestScrollBox->AddChild(QuestEntryWidget);
			QuestEntryWidget->OnQuestSelected.AddDynamic(this, &ThisClass::OnQuestSelected);
		}
	}
}

void UQuestLogWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UQuestLogWidget::OnQuestSelected(FName QuestIDToSet, ARPGQuestSystemActor* QuestActorToSet)
{
	DisplayQuestToRightDetail(QuestIDToSet, QuestActorToSet);
}

void UQuestLogWidget::DisplayQuestToRightDetail(FName QuestIDToSet, ARPGQuestSystemActor* QuestActorToSet)
{
	CurrentQuestActor = QuestActorToSet;
	
	ObjectiveVerticalBox->ClearChildren();
	
	FQuest QuestDetail =GetWorld()->GetGameInstance()->GetSubsystem<UQuestManager>()->GetQuestFromDataTable(QuestIDToSet);
	if (QuestDetail.QuestStages.IsValidIndex(0))
	{
		QuestName->SetText(QuestDetail.QuestName);
		QuestDescription->SetText(QuestDetail.QuestDescription);
		StageDescription->SetText(QuestDetail.QuestStages[0].StageDescription);
		QuestGold->SetText(FText::AsNumber(QuestDetail.QuestStages[0].GoldReward));
	}
	
	RightQuestDetailWidgetSwitcher->SetActiveWidgetIndex(1);

	for (FObjectiveDetail ObjectiveDetail : QuestDetail.QuestStages[0].Objectives)
	{
		UUserWidget* Widget = CreateWidget(GetWorld(), LogEntry_ObjectiveWidgetClass);
		UQuestEntry_Objective* ObjectiveWidget = Cast<UQuestEntry_Objective>(Widget);
		if (ObjectiveWidget)
		{
			ObjectiveWidget->SetObjectiveData(ObjectiveDetail, CurrentQuestActor);
			ObjectiveVerticalBox->AddChild(ObjectiveWidget);
		} 
	}
}
