// LJS


#include "GameMode/GameManager/ItemManager.h"

#include "Character/NPC/NPC_HumanNPC.h"
#include "Character/Player/PlayerCharacter_Fighter.h"
#include "Component/Player/PlayerEquipmentComponent.h"
#include "Component/InventoryComponent.h"
#include "DataAsset/Item/DataAsset_RPGItemData.h"

UItemManager::UItemManager()
{
	
}

void UItemManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UItemManager::Deinitialize()
{
	Super::Deinitialize();
}

bool UItemManager::IsInventorySlotEmpty(FInventorySlot InventorySlot)
{
	// 아이템 데이터 에셋이 유효하지 않으면 비어있음
	if (!InventorySlot.ItemDataAsset.IsValid()) return true;

	return false;
}

bool UItemManager::IsInventorySlotNotEmpty(FInventorySlot InventorySlot)
{
	return !IsInventorySlotEmpty(InventorySlot);
}

FInventorySlot UItemManager::MakeItemToAdd(TSoftObjectPtr<UDataAsset_RPGItemData> ItemToAdd, int32 Quantity)
{
	// 아이템 데이터 어셋 로딩해서 인벤토리 슬롯 생성
	UDataAsset_RPGItemData* const Item = ItemToAdd.LoadSynchronous();
	if (!IsValid(Item)) return FInventorySlot();
	
	FInventorySlot MakeInventorySlot;
	MakeInventorySlot.ItemID = Item->GetItemID();
	MakeInventorySlot.Quantity = Quantity;
	MakeInventorySlot.ItemDataAsset = ItemToAdd;
	
	return MakeInventorySlot;
}

void UItemManager::SetSlotIndex(FInventorySlot InventorySlottoSet, int32 Index)
{
	InventorySlottoSet.SlotIndex = Index;
}

bool UItemManager::IsStackableAndIsEqualAndHaveSpace(FInventorySlot& TargetSlot, FInventorySlot& SlotToStack)
{
	// 타겟 슬롯이 비어있는지 확인
	const bool IsEmpty = IsInventorySlotEmpty(TargetSlot);
	
	if (IsEmpty)
	{
		 return false;
	}
	else
	{
		// 타겟 슬롯이 Stackable인지
		UDataAsset_RPGItemData* TargetSlotItem = TargetSlot.ItemDataAsset.LoadSynchronous();
		bool TargetIsStackalbe = TargetSlotItem->IsStackable();
		if (TargetIsStackalbe)
		{
			// 타겟 슬롯 아이템이 같은 타입인지
			if (TargetSlot.ItemID == SlotToStack.ItemID)
			{
				// 타겟 슬롯의 스택 사이즈와 실제 아이템 양 비교
				if (TargetSlotItem->StackSize > TargetSlot.Quantity)
				{
					return true;
				}
			}
		}		
	}

	return false;
}

void UItemManager::OnInventorySlotDrop(UInventoryComponent* FromContainerInventoryComponent, UInventoryComponent* ToInventoryComponent, int32 FromIndex, int32 ToIndex) const
{
	if (!IsValid(FromContainerInventoryComponent)) return;

	// 드롭 타입 확인
	const bool bIsPlayerToStore = CheckIfPlayerToStore(FromContainerInventoryComponent, ToInventoryComponent);
	const bool bIsToEquipment = CheckIfToEquipment(FromContainerInventoryComponent);
	
	// 플레이어에서 상점으로 드롭 (아이템 판매)
	if (bIsPlayerToStore)
	{
		HandlePlayerToStoreDrop(FromContainerInventoryComponent, FromIndex);
		return;
	}

	// 장비로 드롭
	if (bIsToEquipment)
	{
		HandleEquipmentDrop(FromContainerInventoryComponent, ToInventoryComponent, FromIndex, ToIndex);
		return;
	}
	
	// 일반 아이템 전송
	FromContainerInventoryComponent->TransferItem(ToInventoryComponent, FromIndex, ToIndex);
}

int32 UItemManager::GetStackSize(FInventorySlot TargetSlot)
{
	UDataAsset_RPGItemData* TargetSlotItem = TargetSlot.ItemDataAsset.LoadSynchronous();

	return TargetSlotItem->StackSize;
}

bool UItemManager::CheckIfPlayerToStore(UInventoryComponent* FromContainerInventoryComponent, UInventoryComponent* ToInventoryComponent) const
{
	UPlayerEquipmentComponent* const PlayerEquipmentComponent = Cast<UPlayerEquipmentComponent>(FromContainerInventoryComponent);
	return IsValid(PlayerEquipmentComponent);
}

bool UItemManager::CheckIfToEquipment(UInventoryComponent* FromContainerInventoryComponent) const
{
	UPlayerEquipmentComponent* const PlayerEquipmentComponent = Cast<UPlayerEquipmentComponent>(FromContainerInventoryComponent);
	return IsValid(PlayerEquipmentComponent);
}

void UItemManager::HandlePlayerToStoreDrop(UInventoryComponent* FromContainerInventoryComponent, int32 FromIndex) const
{
	const bool bIsValidSlotIndex = FromContainerInventoryComponent->IsValidSlotIndex(FromIndex);
	if (!bIsValidSlotIndex) return;

	FInventorySlot& ItemSlot = FromContainerInventoryComponent->DefaultItemSlots[FromIndex];
	
	const bool bIsItemDataAssetValid = ItemSlot.ItemDataAsset.IsValid();
	if (!bIsItemDataAssetValid) return;

	UDataAsset_RPGItemData* const ItemData = ItemSlot.ItemDataAsset.LoadSynchronous();
	if (!IsValid(ItemData)) return;

	// 판매 가격 계산 및 골드 추가
	const int32 SellPrice = ItemData->GoldValue;
	FromContainerInventoryComponent->SetGold(SellPrice);
	
	// 아이템 수량에 따른 처리
	const bool bIsQuantityGreaterThanOne = (ItemSlot.Quantity > 1);
	if (bIsQuantityGreaterThanOne)
	{
		// 수량이 1보다 크면 1개 감소
		FInventorySlot ModifiedSlot = ItemSlot;
		FromContainerInventoryComponent->SetQuantityAtSlot(ModifiedSlot, ItemSlot.Quantity - 1);
	}
	else
	{
		// 수량이 1이면 슬롯에서 완전 제거
		FromContainerInventoryComponent->RemoveItemToIndex(FromIndex);
	}
}

void UItemManager::HandleEquipmentDrop(UInventoryComponent* FromContainerInventoryComponent,UInventoryComponent* ToInventoryComponent, int32 FromIndex, int32 ToIndex) const
{
	// TODO: 장비 드롭 로직 구현
	
	// 임시로 일반 전송 처리
	FromContainerInventoryComponent->TransferItem(ToInventoryComponent, FromIndex, ToIndex);
}
