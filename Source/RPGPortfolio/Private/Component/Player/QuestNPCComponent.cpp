// LJS


#include "Component/Player/QuestNPCComponent.h"

#include "Blueprint/UserWidget.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "GameMode/GameManager/QuestManager.h"
#include "GameMode/GameManager/UIManager.h"
#include "Quest/RPGQuestSystemActor.h"
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

void UQuestNPCComponent::DisplayRewards()
{
	TSubclassOf<UUserWidget> QuestWidgetClass = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->GetQuestWidgetClass(EQuestUICategory::Reward);
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


void UQuestNPCComponent::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerCharacterBase* Owner = Cast<APlayerCharacterBase>(GetOwner()))
	{
		OwnerObjectiveID = Owner->GetObjectiveName();
	}
}

FString UQuestNPCComponent::InteractWith()
{
	UQuestManager* QuestManager = GetWorld()->GetGameInstance()->GetSubsystem<UQuestManager>();	
	TArray<FName> ActiveQuests = QuestManager->GetCurrentActiveQuests();	
	TArray<FName> CompletedQuests = QuestManager->GetCompletedQuests();
	
	if (!QuestManager->QueryActiveQuest(QuestData.RowName))
	{
		DisplayQuest();
	}
	else
	{		
		FQuest* QuestDataRow = nullptr;
		if (QuestData.DataTable && !QuestData.RowName.IsNone())
		{
			QuestDataRow = QuestData.GetRow<FQuest>(TEXT("GetQuestData"));
			if (QuestDataRow)
			{
				ARPGQuestSystemActor* Quest = QuestManager->GetQuestActor(QuestData.RowName);
				if (Quest)
				{
					bool IsComplete = Quest->GetIsComplete();
					
					if (IsComplete)
					{
						DisplayRewards();
						return OwnerObjectiveID;	
					}
				}
			}
		}
	}

	return OwnerObjectiveID;
}

void UQuestNPCComponent::LookAt()
{
}

