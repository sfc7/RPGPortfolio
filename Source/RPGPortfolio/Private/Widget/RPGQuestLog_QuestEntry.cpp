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

	if (IsCompleteBox)
	{
		SelectButton->OnClicked.AddDynamic(this, &URPGQuestLog_QuestEntry::OnAcceptButtonClicked);
	}


}

void URPGQuestLog_QuestEntry::NativePreConstruct()
{
	Super::NativePreConstruct();

	
}

void URPGQuestLog_QuestEntry::OnAcceptButtonClicked()
{
	OnQuestSelected.Broadcast(QuestID, QuestActor);
}

void URPGQuestLog_QuestEntry::SetQuestID(FName QuestIDToSet)
{
	QuestID = QuestIDToSet;

	FQuest Quest = GetWorld()->GetGameInstance()->GetSubsystem<UQuestManager>()->GetQuestFromDataTable(QuestID);
	QuestName->SetText(Quest.QuestName);
}

void URPGQuestLog_QuestEntry::SetQuestActor(ARPGQuestSystemActor* QuestActorToSet)
{
	QuestActor = QuestActorToSet;
}
