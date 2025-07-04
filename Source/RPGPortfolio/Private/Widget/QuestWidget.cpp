// LJS


#include "Widget/QuestWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridSlot.h"
#include "GameMode/GameManager/UIManager.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/ItemSlotMaster.h"
#include "Widget/ItemSlotContainer.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "DataAsset/Item/DataAsset_RPGItemData.h"
void UQuestWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (AcceptButton)
	{
		AcceptButton->OnClicked.AddDynamic(this, &UQuestWidget::OnAcceptButtonClicked);
	}
}

void UQuestWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	QuestName->SetText(	QuestDetails.QuestName);
	QuestDescription->SetText(	QuestDetails.QuestDescription);
	if (StageDescription)
	{
		if (QuestDetails.QuestStages.IsValidIndex(0))
		{
			StageDescription->SetText(QuestDetails.QuestStages[0].StageDescription);
			QuestGold->SetText(FText::AsNumber(QuestDetails.QuestStages[0].GoldReward));
			CreateRewardItemSlots();
		}
		else
		{
			StageDescription->SetText(FText::FromString("No Stage Available"));
		}
	}
}

void UQuestWidget::SetQuest(FQuest QuestDetailsToSet)
{
	QuestDetails = QuestDetailsToSet;
}

void UQuestWidget::SetQuestID(FName QuestIDToSet)
{
	QuestID = QuestIDToSet;
}

void UQuestWidget::OnAcceptButtonClicked()
{
	GetWorld()->GetGameInstance()->GetSubsystem<UQuestManager>()->AddNewQuest(QuestID);
	GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->ToggleInputMode(GetWorld(), ERPGInputMode::GameMode);
	RemoveFromParent();
}

void UQuestWidget::CreateRewardItemSlots()
{    
	RewardItemContainer->GridForSlots->ClearChildren();

	TMap<TSoftObjectPtr<UDataAsset_RPGItemData>, int32> Rewards = QuestDetails.QuestStages[0].ItemRewardAndQuantity;
    
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
