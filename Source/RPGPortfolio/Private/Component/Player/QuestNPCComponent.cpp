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


// Called when the game starts or when spawned
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
	// 퀘스트 매니저 상태 확인 로그 추가
	UQuestManager* QuestManager = GetWorld()->GetGameInstance()->GetSubsystem<UQuestManager>();
	
	// 현재 활성 퀘스트 목록
	TArray<FName> ActiveQuests = QuestManager->GetCurrentActiveQuests();
	UE_LOG(LogTemp, Warning, TEXT("=== CURRENT ACTIVE QUESTS ==="));
	UE_LOG(LogTemp, Warning, TEXT("Active Quests Count: %d"), ActiveQuests.Num());
	for (int32 i = 0; i < ActiveQuests.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Active Quest %d: %s"), i, *ActiveQuests[i].ToString());
	}
	
	// 완료된 퀘스트 목록
	TArray<FName> CompletedQuests = QuestManager->GetCompletedQuests();
	UE_LOG(LogTemp, Warning, TEXT("=== COMPLETED QUESTS ==="));
	UE_LOG(LogTemp, Warning, TEXT("Completed Quests Count: %d"), CompletedQuests.Num());
	for (int32 i = 0; i < CompletedQuests.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Completed Quest %d: %s"), i, *CompletedQuests[i].ToString());
	}
	
	// 현재 퀘스트 액터들
	TArray<ARPGQuestSystemActor*> CurrentQuestActors = QuestManager->GetCurrentQuests();
	UE_LOG(LogTemp, Warning, TEXT("=== CURRENT QUEST ACTORS ==="));
	UE_LOG(LogTemp, Warning, TEXT("Quest Actors Count: %d"), CurrentQuestActors.Num());
	for (int32 i = 0; i < CurrentQuestActors.Num(); i++)
	{
		if (CurrentQuestActors[i] && IsValid(CurrentQuestActors[i]))
		{
			UE_LOG(LogTemp, Warning, TEXT("Quest Actor %d: %s (IsComplete: %s)"), 
				i, 
				*CurrentQuestActors[i]->GetQuestID().ToString(),
				CurrentQuestActors[i]->GetIsComplete() ? TEXT("Yes") : TEXT("No"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Quest Actor %d: INVALID!"), i);
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Current Checking QuestID: %s"), *QuestData.RowName.ToString());
	
	if (!QuestManager->QueryActiveQuest(QuestData.RowName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Quest %s is NOT Active - Displaying New Quest"), *QuestData.RowName.ToString());
		DisplayQuest();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Quest %s IS Active - Checking completion"), *QuestData.RowName.ToString());
		
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
					UE_LOG(LogTemp, Warning, TEXT("Quest %s IsComplete: %s"), 
						*QuestData.RowName.ToString(), IsComplete ? TEXT("Yes") : TEXT("No"));
					
					if (IsComplete)
					{
						UE_LOG(LogTemp, Warning, TEXT("Quest %s COMPLETED - Displaying Rewards"), *QuestData.RowName.ToString());
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

