// LJS


#include "Widget/QuestRewardWidget.h"

void UQuestRewardWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UQuestRewardWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UQuestRewardWidget::OnAcceptButtonClicked()
{
	Super::OnAcceptButtonClicked();

	GetWorld()->GetGameInstance()->GetSubsystem<UQuestManager>()->TurnInQuest(QuestID);
}
