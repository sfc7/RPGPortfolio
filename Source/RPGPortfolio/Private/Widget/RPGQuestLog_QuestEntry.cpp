// LJS


#include "Widget/RPGQuestLog_QuestEntry.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "GameMode/GameManager/QuestManager.h"



void URPGQuestLog_QuestEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SelectButton->OnClicked.AddDynamic(this, &URPGQuestLog_QuestEntry::OnAcceptButtonClicked);
	TrackCheckBox->OnCheckStateChanged.AddDynamic(this, &ThisClass::OnTrackCheckBoxClicked);
}

void URPGQuestLog_QuestEntry::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void URPGQuestLog_QuestEntry::OnTrackCheckBoxClicked(bool IsCheck)
{
	if (IsCheck)
	{
		OnQuestTracked.Broadcast(QuestActor, true);
	}
	else
	{
		OnQuestTracked.Broadcast(QuestActor, false);
	}
}

void URPGQuestLog_QuestEntry::OnAcceptButtonClicked()
{
	OnQuestSelected.Broadcast(QuestID, QuestActor);
}

void URPGQuestLog_QuestEntry::SetQuestID(FName QuestIDToSet)
{
	// 퀘스트 ID 설정
	QuestID = QuestIDToSet;

	// 퀘스트 데이터 로드 후 이름 설정
	FQuest Quest = GetWorld()->GetGameInstance()->GetSubsystem<UQuestManager>()->GetQuestFromDataTable(QuestID);
	QuestName->SetText(Quest.QuestName);
}

void URPGQuestLog_QuestEntry::SetQuestActor(ARPGQuestSystemActor* QuestActorToSet)
{
	QuestActor = QuestActorToSet;
}
