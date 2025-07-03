// LJS


#include "Widget/QuestWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameMode/GameManager/UIManager.h"
#include "Kismet/GameplayStatics.h"

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
		}
		else
		{
			StageDescription->SetText(FText::FromString("No Stage Available"));
		}
	}
	
	QuestGold->SetText(FText::AsNumber(QuestDetails.QuestStages[0].GoldReward));
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


