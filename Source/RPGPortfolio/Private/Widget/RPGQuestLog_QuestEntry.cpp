// LJS


#include "Widget/RPGQuestLog_QuestEntry.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/CheckBox.h"

void URPGQuestLog_QuestEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsCompleteBox)
	{
		ClickButton->OnClicked.AddDynamic(this, &URPGQuestLog_QuestEntry::OnAcceptButtonClicked);
	}
}

void URPGQuestLog_QuestEntry::NativePreConstruct()
{
	Super::NativePreConstruct();

}

void URPGQuestLog_QuestEntry::OnAcceptButtonClicked()
{
}