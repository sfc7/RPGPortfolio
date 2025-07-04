// LJS


#include "Widget/QuestRewardWidget.h"

#include "Character/Player/PlayerCharacterBase.h"

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

	APlayerCharacterBase* Player = Cast<APlayerCharacterBase>(GetOwningPlayerPawn());
	if (!Player)
	{
		return;
	}
	
	GetWorld()->GetGameInstance()->GetSubsystem<UQuestManager>()->TurnInQuest(QuestID);

	bool bRewardsGranted = GetWorld()->GetGameInstance()->GetSubsystem<UQuestManager>()->GrantQuestRewards(QuestID, Player);
}
