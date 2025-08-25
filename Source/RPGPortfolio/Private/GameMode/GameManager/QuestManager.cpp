// LJS


#include "GameMode/GameManager/QuestManager.h"

#include "Quest/RPGQuestSystemActor.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "Component/Player/PlayerInventoryComponent.h"
#include "GameMode/GameManager/ItemManager.h"
#include "DataAsset/Item/DataAsset_RPGItemData.h"

void UQuestManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	QuestDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/MyProject/Data/Quest/DT_QuestTable.DT_QuestTable"));
}

ARPGQuestSystemActor* UQuestManager::AddNewQuest(FName QuestID)
{
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
	CompletedQuests.AddUnique(QuestID);
	CurrentActiveQuests.Remove(QuestID);

	OnQuestCompleted.Broadcast(GetQuestActor(QuestID));
}

bool UQuestManager::QueryActiveQuest(FName QuestID)
{
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
	if (QuestDataTable)
	{
		if (FQuest* QuestData = QuestDataTable->FindRow<FQuest>(QuestID, TEXT("GetQuest")))
		{
			return *QuestData;  
		}
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
	CompleteQuest(QuestID);
}

ARPGQuestSystemActor* UQuestManager::GetQuestActor(FName QuestID)
{
	if (QuestID.IsNone())
	{
		return nullptr;
	}
	
	for (ARPGQuestSystemActor* QuestActor : CurrentQuests)
	{
		if (!QuestActor)
		{
			continue;
		}
		
		if (QuestActor->GetQuestID() == QuestID)
		{
			return QuestActor;
		}
	}

	return nullptr;
}

bool UQuestManager::GrantQuestRewards(FName QuestID, APlayerCharacterBase* Player)
{
	if (!Player || !Player->GetPlayerInventoryComponent())
	{
		return false;
	}

	FQuest QuestData = GetQuestFromDataTable(QuestID);
	if (!QuestData.QuestStages.IsValidIndex(0))
	{
		return false;
	}

	UPlayerInventoryComponent* PlayerInventory = Player->GetPlayerInventoryComponent();
    
	bool bItemsGranted = GrantItemRewards(QuestData.QuestStages[0].ItemRewardAndQuantity, PlayerInventory);
	GrantGoldReward(QuestData.QuestStages[0].GoldReward, PlayerInventory);
    
	return bItemsGranted;
}

bool UQuestManager::GrantItemRewards(const TMap<TSoftObjectPtr<UDataAsset_RPGItemData>, int32>& ItemRewards, UPlayerInventoryComponent* PlayerInventory)
{
	if (!PlayerInventory)
	{
		return false;
	}

	bool bAllItemsAdded = true;

	for (auto& RewardPair : ItemRewards)
	{
		TSoftObjectPtr<UDataAsset_RPGItemData> ItemData = RewardPair.Key;
		int32 Quantity = RewardPair.Value;

		if (!ItemData || Quantity <= 0)
		{
			continue;
		}
		
		FInventorySlot ItemToAdd = GetGameInstance()->GetSubsystem<UItemManager>()->MakeItemToAdd(ItemData, Quantity);
        
		if (!PlayerInventory->AddItem(ItemToAdd))
		{
			bAllItemsAdded = false;
		}
	}

	return bAllItemsAdded;
}

void UQuestManager::GrantGoldReward(int32 GoldAmount, UPlayerInventoryComponent* PlayerInventory)
{
	if (!PlayerInventory || GoldAmount <= 0)
	{
		return;
	}

	PlayerInventory->SetGold(GoldAmount);
}
