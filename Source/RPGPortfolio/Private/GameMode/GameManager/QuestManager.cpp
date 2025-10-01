// LJS


#include "GameMode/GameManager/QuestManager.h"

#include "Quest/RPGQuestSystemActor.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "Component/InventoryComponent.h"
#include "GameMode/GameManager/ItemManager.h"
#include "DataAsset/Item/DataAsset_RPGItemData.h"
#include "Component/Player/PlayerInventoryComponent.h"

void UQuestManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	QuestDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/MyProject/Data/Quest/DT_QuestTable.DT_QuestTable"));
}

ARPGQuestSystemActor* UQuestManager::AddNewQuest(FName QuestID)
{
	if (QuestID.IsNone()) return nullptr;

	// 이미 존재하는 퀘스트인지 확인
	for (ARPGQuestSystemActor* ExistingQuest : CurrentQuests)
	{
		if (ExistingQuest && ExistingQuest->GetQuestID() == QuestID)
		{
			return ExistingQuest; 
		}
	}
    
	CurrentActiveQuests.AddUnique(QuestID);

	ARPGQuestSystemActor* QuestActor = GetWorld()->SpawnActorDeferred<ARPGQuestSystemActor>(
		ARPGQuestSystemActor::StaticClass(),
		FTransform(FVector::ZeroVector), 
		nullptr,
		nullptr
	);

	// 퀘스트 액터 설정 및 스폰 완료
	if (QuestActor)
	{
		QuestActor->SetQuestID(QuestID);
		QuestActor->FinishSpawning(FTransform(FVector::ZeroVector));
		CurrentQuests.Add(QuestActor);
	}

	return QuestActor;
}

void UQuestManager::CompleteQuest(FName QuestID)
{
	if (QuestID.IsNone()) return;

	// 완료된 퀘스트 목록에 추가, 활성 퀘스트 목록에서 제거
	CompletedQuests.AddUnique(QuestID);
	CurrentActiveQuests.Remove(QuestID);

	OnQuestCompleted.Broadcast(GetQuestActor(QuestID));
}

bool UQuestManager::QueryActiveQuest(FName QuestID)
{
	if (QuestID.IsNone()) return false;

	// 활성 퀘스트 목록에 포함되어 있는지 확인
	if (CurrentActiveQuests.Contains(QuestID)) return true;

	return false;
}

void UQuestManager::TrackQuest()
{
}

UDataTable* UQuestManager::GetQuestDataTable()
{
	return QuestDataTable;
}

FQuest UQuestManager::GetQuestFromDataTable(FName QuestID)
{
	if (QuestID.IsNone()) return FQuest();

	if (!IsValid(QuestDataTable)) return FQuest();

	// 데이터 테이블에서 퀘스트 데이터 찾기
	if (FQuest* QuestData = QuestDataTable->FindRow<FQuest>(QuestID, TEXT("GetQuest")))
	{
		return *QuestData;  
	}
	
	return FQuest();  
}

TArray<ARPGQuestSystemActor*> UQuestManager::GetCurrentQuests()
{
	return CurrentQuests;
}

TArray<FName> UQuestManager::GetCurrentActiveQuests()
{
	return CurrentActiveQuests;
}

TArray<FName> UQuestManager::GetCompletedQuests()
{
	return CompletedQuests;
}

void UQuestManager::SetCurrentActiveQuests(TArray<FName> CurrentActiveQuestsToSet)
{
	CurrentActiveQuests = CurrentActiveQuestsToSet;
}

void UQuestManager::SetCompleteActiveQuests(TArray<FName> CompletedQuestsToSet)
{
	CompletedQuests = CompletedQuestsToSet;
}

void UQuestManager::ClearAllQuests()
{
	// 모든 퀘스트 액터 제거
	for (ARPGQuestSystemActor* QuestActor : CurrentQuests)
	{
		if (QuestActor && IsValid(QuestActor))
		{
			QuestActor->Destroy();
		}
	}
    
	CurrentQuests.Empty();
	CurrentActiveQuests.Empty();
}

void UQuestManager::TurnInQuest(FName QuestID)
{
	// 퀘스트 완료 처리
	CompleteQuest(QuestID);
}

ARPGQuestSystemActor* UQuestManager::GetQuestActor(FName QuestID)
{
	if (QuestID.IsNone()) return nullptr;

	// 현재 퀘스트들에서 해당 ID의 퀘스트 액터 찾기
	for (ARPGQuestSystemActor* QuestActor : CurrentQuests)
	{
		if (!QuestActor) continue;
		
		if (QuestActor->GetQuestID() == QuestID)
		{
			return QuestActor;
		}
	}

	return nullptr;
}

bool UQuestManager::GrantQuestRewards(FName QuestID, APlayerCharacterBase* Player)
{
	if (!IsValid(Player)) return false;

	UPlayerInventoryComponent* const PlayerInventoryComponent = Player->GetPlayerInventoryComponent();
	if (!IsValid(PlayerInventoryComponent)) return false;

	// 퀘스트 데이터 테이블에서 가져오기
	FQuest QuestData = GetQuestFromDataTable(QuestID);
	if (!QuestData.QuestStages.IsValidIndex(0))
	{
		return false;
	}

	UInventoryComponent* PlayerInventory = Player->GetPlayerInventoryComponent();

	// 아이템,골드 보상 지급
	bool bItemsGranted = GrantItemRewards(QuestData.QuestStages[0].ItemRewardAndQuantity, PlayerInventory);
	GrantGoldReward(QuestData.QuestStages[0].GoldReward, PlayerInventory);
    
	return bItemsGranted;
}

bool UQuestManager::GrantItemRewards(const TMap<TSoftObjectPtr<UDataAsset_RPGItemData>, int32>& ItemRewards, UInventoryComponent* PlayerInventory)
{
	if (!IsValid(PlayerInventory)) return false;

	// 모든 아이템이 추가 됐는지 확인할 OUT 변수
	bool bAllItemsAdded = true;

	// 각 아이템 보상 처리
	for (auto& RewardPair : ItemRewards)
	{
		// 아이템 데이터 및 수량 유효성 검사
		TSoftObjectPtr<UDataAsset_RPGItemData> ItemData = RewardPair.Key;
		int32 Quantity = RewardPair.Value;

		if (!ItemData || Quantity <= 0)
		{
			continue;
		}
		
		// 인벤토리에 아이템 추가 시도
		FInventorySlot ItemToAdd = GetGameInstance()->GetSubsystem<UItemManager>()->MakeItemToAdd(ItemData, Quantity);
		
		if (!PlayerInventory->AddItem(ItemToAdd))
		{
			bAllItemsAdded = false;
		}
	}

	return bAllItemsAdded;
}

void UQuestManager::GrantGoldReward(int32 GoldAmount, UInventoryComponent* PlayerInventory)
{
	if (!IsValid(PlayerInventory)) return;
	if (GoldAmount <= 0) return;

	PlayerInventory->SetGold(GoldAmount);
}
