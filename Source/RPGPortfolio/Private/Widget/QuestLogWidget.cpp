// LJS


#include "Widget/QuestLogWidget.h"

#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"
#include "GameMode/GameManager/QuestManager.h"
#include "GameMode/GameManager/UIManager.h"
#include "Quest/RPGQuestSystemActor.h"
#include "Widget/ItemSlotMaster.h"
#include "Widget/RPGQuestLog_QuestEntry.h"
#include "Widget/QuestEntry_Objective.h"
#include "Widget/ItemSlotMaster.h"
#include "Widget/ItemSlotContainer.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "DataAsset/Item/DataAsset_RPGItemData.h"
#include "Widget/QuestTracker.h"

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
			QuestEntryWidget->OnQuestTracked.AddDynamic(this, &ThisClass::OnQuestTracked);
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

void UQuestLogWidget::OnQuestTracked(ARPGQuestSystemActor* QuestActorToSet, bool IsCheck)
{
	if (IsCheck)
	{
		if (IsValid(TrackWidget))
		{
			TrackWidget->UpdateQuestActor(QuestActorToSet);
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Quest Tracked1"));
			UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), Quest_TrackWidgetClass);
			TrackWidget = Cast<UQuestTracker>(Widget);
			TrackWidget->SetQuestActor(QuestActorToSet);
			GetGameInstance()->GetSubsystem<UQuestManager>()->OnQuestCompleted.AddDynamic(TrackWidget, &UQuestTracker::QuestCompleted);
			if (TrackWidget)
			{
				TrackWidget->AddToViewport(-1);
			}
		}
	}
	else
	{
		TrackWidget->RemoveFromParent();
		TrackWidget = nullptr;
	}
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
		CreateRewardItemSlots(QuestDetail);
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

void UQuestLogWidget::CreateRewardItemSlots(FQuest QuestDetail)
{
	RewardItemContainer->GridForSlots->ClearChildren();
    
	TMap<TSoftObjectPtr<UDataAsset_RPGItemData>, int32> Rewards = QuestDetail.QuestStages[0].ItemRewardAndQuantity;
    
	int32 Index = 0;
	for (auto& Pair : Rewards)
	{
		if (!Pair.Key) continue;
		UItemSlotMaster* ItemSlot = CreateWidget<UItemSlotMaster>(GetWorld(), ItemSlotMasterClass);

		FInventorySlot SlotData;
		SlotData.ItemDataAsset = Pair.Key;
		SlotData.Quantity = Pair.Value;
		SlotData.ItemID = Pair.Key->GetItemID();
        
		ItemSlot->UpdateSlotData(SlotData);

		int32 Row = Index / 4;
		int32 LastColumn = Index % 4;
		
		RewardItemContainer->GridForSlots->AddChildToUniformGrid(ItemSlot, Row, LastColumn);
        
		Index++;
	}
}
