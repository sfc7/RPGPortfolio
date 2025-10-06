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

	// 현재 퀘스트들에 대해 엔트리 위젯 생성
	for (ARPGQuestSystemActor* CurrentQuest : CurrentQuests)
	{
		FName CurrentQuestID = CurrentQuest->GetQuestID();
		if (!GetWorld()->GetGameInstance()->GetSubsystem<UQuestManager>()->QueryActiveQuest(CurrentQuestID)) continue;
		
		FQuest QuestDetail =GetWorld()->GetGameInstance()->GetSubsystem<UQuestManager>()->GetQuestFromDataTable(CurrentQuestID);
		UUserWidget* Widget = CreateWidget(GetWorld(), Quest_LogEntryWidgetClass);
		URPGQuestLog_QuestEntry* QuestEntryWidget = Cast<URPGQuestLog_QuestEntry>(Widget);
		if (QuestEntryWidget)
		{
			// 퀘스트 엔트리 설정 후 스크롤 박스에 추구하여 엔트리 구현
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
	// 트래킹 추적 박스가 Check 되어있는지에 따라 구현
	if (IsCheck)
	{
		// 추적 위젯이 이미 있는 경우는 업데이트
		if (IsValid(TrackWidget))
		{
			TrackWidget->UpdateQuestActor(QuestActorToSet);
		}
		// 추적 위젯이 이미 없는 경우 새로 생성
		else
		{
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
	// 트래킹 추적 박스 해제시 삭제
	else
	{
		if (IsValid(TrackWidget))
		{
			TrackWidget->RemoveFromParent();
			TrackWidget = nullptr;
		}
	}
}

void UQuestLogWidget::DisplayQuestToRightDetail(FName QuestIDToSet, ARPGQuestSystemActor* QuestActorToSet)
{
	// 현재 퀘스트 액터 설정
	CurrentQuestActor = QuestActorToSet;

	// 목표 박스 정리
	ObjectiveVerticalBox->ClearChildren();

	//@ 퀘스트의 첫 스테이지가 있는지로 유효성 판단후 퀘스트 정보 세팅
	FQuest QuestDetail =GetWorld()->GetGameInstance()->GetSubsystem<UQuestManager>()->GetQuestFromDataTable(QuestIDToSet);
	if (QuestDetail.QuestStages.IsValidIndex(0))
	{
		QuestName->SetText(QuestDetail.QuestName);
		QuestDescription->SetText(QuestDetail.QuestDescription);
		StageDescription->SetText(QuestDetail.QuestStages[0].StageDescription);
		QuestGold->SetText(FText::AsNumber(QuestDetail.QuestStages[0].GoldReward));
		CreateRewardItemSlots(QuestDetail);
	}

	// 오른쪽 Switcher에 선택한 퀘스트 정보 표시
	RightQuestDetailWidgetSwitcher->SetActiveWidgetIndex(1);

	// 퀘스트의 모든 스테이지들 표시
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
	// 기존 슬롯 정리
	RewardItemContainer->GridForSlots->ClearChildren();

	// 보상 아이템 맵 가져오기
	TMap<TSoftObjectPtr<UDataAsset_RPGItemData>, int32> Rewards = QuestDetail.QuestStages[0].ItemRewardAndQuantity;
    
	int32 Index = 0;
	// 모든 보상 아이템들 슬롯을 만들어서 아이템 컨테이너에 추가
	for (auto& Pair : Rewards)
	{
		if (!Pair.Key) continue;
		UItemSlotMaster* ItemSlot = CreateWidget<UItemSlotMaster>(GetWorld(), ItemSlotMasterClass);

		FInventorySlot SlotData;
		SlotData.ItemDataAsset = Pair.Key;
		SlotData.Quantity = Pair.Value;
		SlotData.ItemID = Pair.Key->GetItemID();
        
		ItemSlot->UpdateSlotData(SlotData);

		// 그리드 위치 계산
		int32 Row = Index / 4;
		int32 LastColumn = Index % 4;
		
		RewardItemContainer->GridForSlots->AddChildToUniformGrid(ItemSlot, Row, LastColumn);
        
		Index++;
	}
}
