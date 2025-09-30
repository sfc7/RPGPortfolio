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
	UUIManager* const UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
	if (!IsValid(UIManager)) return;

	const TSubclassOf<UUserWidget> QuestWidgetClass = UIManager->GetQuestWidgetClass(EQuestUICategory::NPC);
	if (!IsValid(QuestWidgetClass)) return;
    
	UUserWidget* const Widget = CreateWidget<UUserWidget>(GetWorld(), QuestWidgetClass);
	UQuestWidget* const QuestWidget = Cast<UQuestWidget>(Widget);
	if (!IsValid(QuestWidget)) return;

	// 퀘스트 데이터 가져오기
	FQuest* QuestDataRow = nullptr;
	if (QuestData.DataTable && !QuestData.RowName.IsNone())
	{
		QuestDataRow = QuestData.GetRow<FQuest>(TEXT("GetQuestData"));
	}
    
	// 퀘스트 데이터가 있으면 UI 설정, 없으면 제거
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
	UUIManager* const UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
	if (!IsValid(UIManager)) return;

	const TSubclassOf<UUserWidget> QuestWidgetClass = UIManager->GetQuestWidgetClass(EQuestUICategory::Reward);
	if (!IsValid(QuestWidgetClass)) return;

	UUserWidget* const Widget = CreateWidget<UUserWidget>(GetWorld(), QuestWidgetClass);
	UQuestWidget* const QuestWidget = Cast<UQuestWidget>(Widget);
	if (!IsValid(QuestWidget)) return;

	// 퀘스트 데이터 가져오기
	FQuest* QuestDataRow = nullptr;
	if (QuestData.DataTable && !QuestData.RowName.IsNone())
	{
		QuestDataRow = QuestData.GetRow<FQuest>(TEXT("GetQuestData"));
	}

	// 퀘스트 데이터가 있으면 UI 설정, 없으면 제거
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

	APlayerCharacterBase* Owner = Cast<APlayerCharacterBase>(GetOwner());
	if (IsValid(Owner))
	{
		OwnerObjectiveID = Owner->GetObjectiveName();
	}
}

FString UQuestNPCComponent::InteractWith()
{
	UQuestManager* const QuestManager = GetWorld()->GetGameInstance()->GetSubsystem<UQuestManager>();	
	if (!IsValid(QuestManager)) return OwnerObjectiveID;

	// 현재 활성 퀘스트와 완료된 퀘스트 목록 가져오기
	const TArray<FName> ActiveQuests = QuestManager->GetCurrentActiveQuests();	
	const TArray<FName> CompletedQuests = QuestManager->GetCompletedQuests();
	
	// 퀘스트가 활성화되지 않은 경우 퀘스트 수락 UI 표시
	const bool bIsQuestActive = QuestManager->QueryActiveQuest(QuestData.RowName);
	if (!bIsQuestActive)
	{
		DisplayQuest();
		return OwnerObjectiveID;
	}

	// 활성화된 퀘스트가 있는 경우 완료 상태 확인
	FQuest* QuestDataRow = nullptr;
	if (QuestData.DataTable && !QuestData.RowName.IsNone())
	{
		QuestDataRow = QuestData.GetRow<FQuest>(TEXT("GetQuestData"));
	}
	
	if (!QuestDataRow) return OwnerObjectiveID;

	// 퀘스트 액터 가져오기
	ARPGQuestSystemActor* const Quest = QuestManager->GetQuestActor(QuestData.RowName);
	if (!IsValid(Quest)) return OwnerObjectiveID;

	// 퀘스트 완료 상태 확인
	const bool bIsComplete = Quest->GetIsComplete();
	if (bIsComplete)
	{
		// 완료시 퀘스트의 보상 UI 표시
		DisplayRewards();
	}

	return OwnerObjectiveID;
}

void UQuestNPCComponent::DisplayConversation()
{
	if (!GetWorld()) return;
	
	if (!IsValid(ConversationWidgetClass)) return;
	
	UUserWidget* const Widget = CreateWidget<UUserWidget>(GetWorld(), ConversationWidgetClass);
	if (!IsValid(Widget)) return;

	// 대화 위젯을 화면에 표시
	Widget->AddToViewport();
}

