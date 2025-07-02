// LJS


#include "Component/Player/QuestNPCComponent.h"

#include "Blueprint/UserWidget.h"
#include "GameMode/GameManager/QuestManager.h"
#include "GameMode/GameManager/UIManager.h"
#include "Widget/QuestWidget.h"


UQuestNPCComponent::UQuestNPCComponent()
{
}

void UQuestNPCComponent::DisplayQuest()
{
	TSubclassOf<UUserWidget> QuestWidgetClass = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->GetQuestWidgetClass(EQuestUICategory::NPC);
	if (!IsValid(QuestWidgetClass))
	{
		return;
	}
    
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), QuestWidgetClass);
	UQuestWidget* QuestWidget = Cast<UQuestWidget>(Widget);
    
	if (!QuestWidget)
	{
		return;
	}

	FQuest* QuestDataRow = nullptr;
	if (QuestData.DataTable && !QuestData.RowName.IsNone())
	{
		QuestDataRow = QuestData.GetRow<FQuest>(TEXT("GetQuestData"));
	}
    
	if (QuestDataRow)
	{
		QuestWidget->SetQuest(*QuestDataRow);
		QuestWidget->SetQuestID(QuestData.RowName);
		QuestWidget->AddToViewport();
	}
	else
	{
		QuestWidget->RemoveFromParent();
	}
}


// Called when the game starts or when spawned
void UQuestNPCComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

FString UQuestNPCComponent::InteractWith()
{
	if (!GetWorld()->GetGameInstance()->GetSubsystem<UQuestManager>()->QueryActiveQuest(QuestData.RowName))
	{
		DisplayQuest();
		return FString();
	}

	return FString();
}

void UQuestNPCComponent::LookAt()
{
}

