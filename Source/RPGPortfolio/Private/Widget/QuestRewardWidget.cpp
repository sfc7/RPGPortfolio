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
	if (!IsValid(Player)) return;

	// 퀘스트를 완료시키고
	GetWorld()->GetGameInstance()->GetSubsystem<UQuestManager>()->TurnInQuest(QuestID);

	// 퀘스트 보상 수령 진행
	bool bRewardsGranted = GetWorld()->GetGameInstance()->GetSubsystem<UQuestManager>()->GrantQuestRewards(QuestID, Player);
}
