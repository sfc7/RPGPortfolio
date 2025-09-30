// LJS

#include "Component/InventoryComponent.h"

#include "GameMode/GameManager/ItemManager.h"
#include "Kismet/KismetArrayLibrary.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "DataAsset/Item/DataAsset_RPGItemData.h"
#include "GameMode/GameManager/UIManager.h"
#include "Component/Player/PlayerEquipmentComponent.h"
#include "Item/InventoryTypeStrategy.h"
#include "Item/InventorySituationStrategy.h"

UInventoryComponent::UInventoryComponent()
{
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ItemManager = GetWorld()->GetGameInstance()->GetSubsystem<UItemManager>();
	
	SetDefaultInventoryTypeStrategy();
	SetDefaultInventorySituationStrategy();
	
	SetupSlots(SlotAmounts);
}

void UInventoryComponent::SetupSlots(int32 SlotAmountstoSetup)
{
	// 슬롯마다 FInventorySlot 설정
	TArray<FInventorySlot>& Slots = GetCurrentItemSlots();
	for (int Index = 0; Index < SlotAmountstoSetup; Index++)
	{
		FInventorySlot InventorySlot;
		InventorySlot.SlotIndex = Index;
		InventorySlot.InventoryRef = this;
		Slots.Add(InventorySlot);
	}
}

bool UInventoryComponent::AddItem(FInventorySlot ItemToAdd)
{
	TArray<FInventorySlot>& Slots = GetCurrentItemSlots();
	FInventorySlot CurrentItemToAdd = ItemToAdd;
	
	UDataAsset_RPGItemData* const CurrentItemData = CurrentItemToAdd.ItemDataAsset.LoadSynchronous();
	if (!IsValid(CurrentItemData)) return false;
	
	// 아이템 Stackable 속성 체크
	const bool bIsStackable = CurrentItemData->IsStackable();
	const int32 CurrentItemStackSize = CurrentItemData->StackSize;
	
	// 스택 가능한 아이템 처리
	if (bIsStackable)
	{
		// 기존 슬롯에 스택 시도
		for (FInventorySlot& TargetSlot : Slots)
		{
			const bool bIsStackableAndIsEqualAndHaveSpace = ItemManager->IsStackableAndIsEqualAndHaveSpace(TargetSlot, CurrentItemToAdd);
			if (bIsStackableAndIsEqualAndHaveSpace)
			{
				const int32 TotalQuantity = TargetSlot.Quantity + CurrentItemToAdd.Quantity;

				if (TotalQuantity > CurrentItemStackSize)
				{
					SetQuantityAtSlot(TargetSlot, CurrentItemStackSize);
					CurrentItemToAdd.Quantity = TotalQuantity - CurrentItemStackSize;
				}
				else
				{
					SetQuantityAtSlot(TargetSlot, TotalQuantity);
					return true;
				}
			}
		}

		// 빈 슬롯에 나머지 아이템 추가
		FInventorySlot FindInventorySlot;
		while (FindEmptySlot(FindInventorySlot))
		{
			if (CurrentItemToAdd.Quantity <= CurrentItemStackSize)
			{
				SetItem(FindInventorySlot, CurrentItemToAdd);
				return true;
			}
			else
			{
				FInventorySlot TempItemToAdd = CurrentItemToAdd;
				TempItemToAdd.Quantity = CurrentItemStackSize;
				SetItem(FindInventorySlot, TempItemToAdd);

				const int32 RemainQuantity = CurrentItemToAdd.Quantity - CurrentItemStackSize;
				CurrentItemToAdd.Quantity = RemainQuantity;
			}
		}
	}
	else
	{
		// 스택 불가능한 아이템 - 빈 슬롯에 바로 추가
		FInventorySlot FindInventorySlot;
		if (FindEmptySlot(FindInventorySlot))
		{
			SetItem(FindInventorySlot, ItemToAdd);
			return true;
		}
	}
	
	return false;
}

bool UInventoryComponent::AddItemToIndex(FInventorySlot ItemToAdd, int32 ToIndex, bool& OutAreAllItemAdded)
{
	if (!IsValidSlotIndex(ToIndex))
	{
		OutAreAllItemAdded = false;
		return false;
	}

	TArray<FInventorySlot>& Slots = GetCurrentItemSlots();
	
	// 빈 슬롯인지 확인 후 SetItem
	if (ItemManager->IsInventorySlotEmpty(Slots[ToIndex]))
	{
		SetItem(Slots[ToIndex], ItemToAdd);
		OutAreAllItemAdded = true;
		return true;
	}
	// 빈 슬롯 아닐 시 스택 아이템 전달
	else
	{
		StackItemOnTransfer(Slots[ToIndex], ItemToAdd, OutAreAllItemAdded);
		return true;
	}
}

bool UInventoryComponent::RemoveItemToIndex(int32 ToIndex)
{
	if (!IsValidSlotIndex(ToIndex)) return false;

	TArray<FInventorySlot>& Slots = GetCurrentItemSlots();
	
	// 해당 Index 빈 슬롯으로 만들기
	FInventorySlot EmptyInventorySlot;
	EmptyInventorySlot.ItemID = FName(TEXT("None"));
	EmptyInventorySlot.Quantity = 0;
	
	SetItem(Slots[ToIndex], EmptyInventorySlot);
	
	return true;
}

FInventorySlot UInventoryComponent::SetQuantityAtSlot(FInventorySlot& TargetSlot, int32 QuantityToSet)
{
	// 슬롯 Index에 Quantity 설정
	TArray<FInventorySlot>& Slots = GetCurrentItemSlots();
	Slots[TargetSlot.SlotIndex].Quantity = QuantityToSet;

	OnInventorySlotChangedDelegate.Broadcast(Slots[TargetSlot.SlotIndex]);

	return Slots[TargetSlot.SlotIndex];
}

bool UInventoryComponent::TransferItem(UInventoryComponent* ToInventoryComponent, int32 FromIndex, int32 ToIndex)
{
	// 같은 인벤토리, 같은 인덱스 체크 
	if (this == ToInventoryComponent && FromIndex == ToIndex) return false;
	
	if (!IsValid(ToInventoryComponent)) return false;
	
	if (!IsValidSlotIndex(FromIndex)) return false;

	// ToIndex가 -1이 아닌 경우 유효성 검사
	if (ToIndex != -1 && !ToInventoryComponent->IsValidSlotIndex(ToIndex)) return false;

	// ToIndex가 -1인 경우 처리 (다른 인벤토리로 이동 구현 예정)
	if (ToIndex == -1) return false;
	
	TArray<FInventorySlot>& Slots = GetCurrentItemSlots();
	bool bAreAllItemsAdded = false;
	
	// 대상 인벤토리에 아이템 추가 시도
	const bool bSuccessAdd = ToInventoryComponent->AddItemToIndex(Slots[FromIndex], ToIndex, bAreAllItemsAdded);
	if (!bSuccessAdd) return false;
	

	// 모든 아이템이 추가된 경우 원본에서 제거
	if (bAreAllItemsAdded)
	{
		return RemoveItemToIndex(FromIndex);
	}

	return true;
}

bool UInventoryComponent::IsValidSlotIndex(int32 FindIndex)
{
	// 현재 아이템 슬롯 인덱스 유효성 확인
	return GetCurrentItemSlots().IsValidIndex(FindIndex);
}

bool UInventoryComponent::StackItemOnTransfer(FInventorySlot TargetSlot, FInventorySlot FromSlot, bool& OutAreAllItemAdded)
{
	// Stackable한지, 같은 종류 아이템인지, 공간이 있는지 확인
	const bool bIsStackableAndIsEqualAndHaveSpace = ItemManager->IsStackableAndIsEqualAndHaveSpace(TargetSlot, FromSlot);
	const int32 ItemStackSize = ItemManager->GetStackSize(TargetSlot);

	if (bIsStackableAndIsEqualAndHaveSpace)
	{
		const int32 TotalQuantity = TargetSlot.Quantity + FromSlot.Quantity;
		
		if (TotalQuantity > ItemStackSize)
		{
			// 스택 크기 초과 - 일부만 스택
			SetQuantityAtSlot(TargetSlot, ItemStackSize);
			SetQuantityAtSlot(FromSlot, TotalQuantity - ItemStackSize);
			
			OutAreAllItemAdded = false;
			return false;
		}
		else
		{
			// 모든 아이템 스택 가능
			SetQuantityAtSlot(TargetSlot, TotalQuantity);
			
			OutAreAllItemAdded = true;
			return true;
		}   
	}
	else
	{
		// 스택 불가능 - 위치 교체
		SwapIndex(TargetSlot, FromSlot);
		OutAreAllItemAdded = false;
	}

	OutAreAllItemAdded = false;
	return false;
}

void UInventoryComponent::SwapIndex(FInventorySlot TargetSlot, FInventorySlot FromSlot)
{
	SetItem(TargetSlot, FromSlot);
	FromSlot.InventoryRef->SetItem(FromSlot, TargetSlot);
}

void UInventoryComponent::EquipItem(FInventorySlot FromSlot)
{
	TArray<FInventorySlot>& Slots = GetCurrentItemSlots();
	const int32 FromIndex = FromSlot.SlotIndex;	
	
	if (!Slots.IsValidIndex(FromIndex)) return;
	
	if (Slots[FromIndex].ItemDataAsset->ItemType != EItemType::Equipment) return;
	
	if (InventoryType != EInventoryType::PlayerInventory) return;
	
	if (!IsValid(EquipmentComponentRef)) return;
	
	// 아이템 장착 및 인벤토리에서 제거
	EquipmentComponentRef->ApplyEquipmentItem(FromSlot);
	RemoveItemToIndex(FromIndex);
}

void UInventoryComponent::UnEquipItem(FInventorySlot FromSlot)
{
	TArray<FInventorySlot>& Slots = GetCurrentItemSlots();
	const int32 FromIndex = FromSlot.SlotIndex;	

	if (!Slots.IsValidIndex(FromIndex)) return;

	if (Slots[FromIndex].ItemDataAsset->ItemType != EItemType::Equipment) return;
	
	if (InventoryType != EInventoryType::Equipment) return;

	if (!IsValid(EquipmentComponentRef)) return;

	// 아이템 해제
	EquipmentComponentRef->ApplyUnEquipmentItem(FromSlot);
}

void UInventoryComponent::SetItem(FInventorySlot TargetSlot, FInventorySlot ItemToSet)
{
	TArray<FInventorySlot>& Slots = GetCurrentItemSlots();
	const int32 TargetIndex = TargetSlot.SlotIndex;	

	if (!Slots.IsValidIndex(TargetIndex)) return;
	

	// 아이템 설정 및 슬롯 정보 업데이트
	Slots[TargetIndex] = ItemToSet;
	Slots[TargetIndex].SlotIndex = TargetIndex;
	Slots[TargetIndex].InventoryRef = this;
	
	OnInventorySlotChangedDelegate.Broadcast(Slots[TargetIndex]);
	
	// 인벤토리 타입별 추가 델리게이트 브로드캐스트
	if (InventoryType == EInventoryType::Potion)
	{
		OnPotionBarSlotChangedDelegate.Broadcast();
	}
	else if (InventoryType == EInventoryType::Equipment)
	{
		OnEquipmentSlotChangedDelegate.Broadcast();
	}
}

void UInventoryComponent::SetGold(int32 GoldAmount)
{
	Gold += GoldAmount;
	OnGoldChangedDelegate.Broadcast(Gold);
}

bool UInventoryComponent::FindEmptySlot(FInventorySlot& OutEmptySlot)
{
	TArray<FInventorySlot>& Slots = GetCurrentItemSlots();
	// 돌면서 비어있는 슬롯 찾기 
	for (FInventorySlot& ItemSlot : Slots)
	{
		const bool bIsEmpty = GetWorld()->GetGameInstance()->GetSubsystem<UItemManager>()->IsInventorySlotEmpty(ItemSlot);
		if (bIsEmpty)
		{
			OutEmptySlot = ItemSlot;
			return true;
		}
	}
	
	return false;
}

TArray<FInventorySlot>& UInventoryComponent::GetCurrentItemSlots()
{
	if (CurrentInventoryTypeStrategy)
	{
		// InventoryType 전략 패턴에 따른 GetSlots
		return CurrentInventoryTypeStrategy->GetSlots(this);
	}
	
	return DefaultItemSlots;
}

const TArray<FInventorySlot>& UInventoryComponent::GetCurrentItemSlots() const
{
	if (CurrentInventoryTypeStrategy)
	{
		// InventoryType 전략 패턴에 따른 GetSlots
		return CurrentInventoryTypeStrategy->GetSlots(this);
	}
	return DefaultItemSlots;
}


void UInventoryComponent::HandleSlotDoubleClick(FInventorySlot& SlotData)
{	
	if (CurrentInventorySituationStrategy)
	{
		// InventorySituation 전략 패턴에 따른 HandleItemDoubleClick
		CurrentInventorySituationStrategy->HandleItemDoubleClick(this, SlotData);
	}
}

void UInventoryComponent::HandleSlotRightClick(FInventorySlot& SlotData)
{
	if (CurrentInventorySituationStrategy)
	{
		// InventorySituation 전략 패턴에 따른 HandleItemRightClick
		CurrentInventorySituationStrategy->HandleItemRightClick(this, SlotData);
	}
}

bool UInventoryComponent::TrySellItem(const FInventorySlot& SlotToSell)
{
	if (InventoryType != EInventoryType::PlayerInventory) return false;
	
	if (!SlotToSell.ItemDataAsset.IsValid() || SlotToSell.Quantity <= 0) return false;
	
	if (!IsValidSlotIndex(SlotToSell.SlotIndex)) return false;

	UDataAsset_RPGItemData* ItemData = SlotToSell.ItemDataAsset.LoadSynchronous();
	if (!ItemData) return false;

	// 판매 가격 계산 및 골드 추가
	const int32 SellPrice = ItemData->GoldValue;
	SetGold(SellPrice);

	// 아이템 수량에 따른 처리
	TArray<FInventorySlot>& Slots = GetCurrentItemSlots();
	FInventorySlot& CurrentSlot = Slots[SlotToSell.SlotIndex];
    
	if (CurrentSlot.Quantity > 1)
	{
		// 수량이 1보다 크면 수량만 감소
		SetQuantityAtSlot(CurrentSlot, CurrentSlot.Quantity - 1);
	}
	else
	{
		// 수량이 1이면 슬롯에서 아이템 완전 제거
		RemoveItemToIndex(SlotToSell.SlotIndex);
	}

	return true;
}

bool UInventoryComponent::TryPurchaseFromStore(const FInventorySlot& StoreSlot, UInventoryComponent* StoreInventory)
{
	if (InventoryType != EInventoryType::PlayerInventory) return false;
	
	if (!StoreInventory) return false;

	if (!CanPurchaseFromStore(StoreSlot)) return false;
		
	if (!StoreInventory->IsValidSlotIndex(StoreSlot.SlotIndex)) return false;
	
	UDataAsset_RPGItemData* ItemData = StoreSlot.ItemDataAsset.LoadSynchronous();
	if (!ItemData) return false;
	

	// 구매할 아이템 생성 (수량 1개)
	FInventorySlot ItemToBuy = StoreSlot;
	ItemToBuy.Quantity = 1;
	ItemToBuy.InventoryRef = this;

	// 플레이어 인벤토리에 아이템 추가 시도
	const bool bItemAdded = AddItem(ItemToBuy);
	if (!bItemAdded) return false; 
	

	// 골드 차감
	const int32 ItemPrice = ItemData->GoldValue;
	SetGold(-ItemPrice);

	// 상점 인벤토리에서 아이템 수량 감소 또는 제거
	TArray<FInventorySlot>& StoreSlots = StoreInventory->GetCurrentItemSlots();
	FInventorySlot& StoreSlotRef = StoreSlots[StoreSlot.SlotIndex];
    
	if (StoreSlotRef.Quantity > 1)
	{
		// 수량이 1보다 크면 수량만 감소
		StoreInventory->SetQuantityAtSlot(StoreSlotRef, StoreSlotRef.Quantity - 1);
	}
	else
	{
		// 수량이 1이면 슬롯에서 아이템 완전 제거
		StoreInventory->RemoveItemToIndex(StoreSlot.SlotIndex);
	}

	return true;
}

bool UInventoryComponent::CanPurchaseFromStore(const FInventorySlot& StoreSlot) const
{
	if (InventoryType != EInventoryType::PlayerInventory) return false;

	if (!StoreSlot.ItemDataAsset.IsValid() || StoreSlot.Quantity <= 0) return false;
	
	UDataAsset_RPGItemData* ItemData = StoreSlot.ItemDataAsset.LoadSynchronous();
	if (!ItemData) return false;
	

	// 골드가 충분한지 확인
	const int32 ItemPrice = ItemData->GoldValue;
	return Gold >= ItemPrice;
}

void UInventoryComponent::SetDefaultInventoryTypeStrategy()
{
	// 아직 전략이 설정되지 않았다면
	if(!CurrentInventoryTypeStrategy)
	{
		// 기본 전략 UObject 생성 (GC 때문에)
		UDefaultTypeStrategy* DefaultInventoryTypeStrategy = NewObject<UDefaultTypeStrategy>(this);
		// UObject를 인터페이스 래핑해서 보관
		CurrentInventoryTypeStrategy = TScriptInterface<IInventoryTypeStrategy>(DefaultInventoryTypeStrategy);
	}
}

void UInventoryComponent::SetDefaultInventorySituationStrategy()
{
	// 아직 전략이 설정되지 않았다면
	if(!CurrentInventorySituationStrategy)
	{
		// 기본 전략 UObject 생성 (GC 때문에)
		UInventorySituationStrategy* DefaultInventorySituationStrategy = NewObject<UInventorySituationStrategy>(this);
		// UObject를 인터페이스 래핑해서 보관
		CurrentInventorySituationStrategy = TScriptInterface<IInventorySituationStrategy>(DefaultInventorySituationStrategy);
	}
}


///////// UDefaultInventoryTypeStrategy


TArray<FInventorySlot>& UDefaultTypeStrategy::GetSlots(UInventoryComponent* OwnerInventory)
{
	check(OwnerInventory);
	return OwnerInventory->DefaultItemSlots;
}
const TArray<FInventorySlot>& UDefaultTypeStrategy::GetSlots(const UInventoryComponent* OwnerInventory) const
{
	check(OwnerInventory);
	return OwnerInventory->DefaultItemSlots;
}


///////// UDefaultInventorySituationStrategy

void UDefaultSituationStrategy::HandleItemDoubleClick(UInventoryComponent* Inventory, FInventorySlot& SlotData)
{
}

void UDefaultSituationStrategy::HandleItemRightClick(UInventoryComponent* Inventory, FInventorySlot& SlotData)
{
}
