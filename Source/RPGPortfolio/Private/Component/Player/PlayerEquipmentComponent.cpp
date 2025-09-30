// LJS


#include "Component/Player/PlayerEquipmentComponent.h"
#include "Component/Player/PlayerInventoryComponent.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "GameAbilitySystem/RPGAttributeSet.h"

UPlayerEquipmentComponent::UPlayerEquipmentComponent()
{
	InventoryType = EInventoryType::Equipment;
}

void UPlayerEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (const APlayerCharacterBase* PlayerCharacter = Cast<APlayerCharacterBase>(GetOwner()))
	{
		PlayerInventoryComponentRef = PlayerCharacter->GetPlayerInventoryComponent();
	}
	
	// 장비칸은 아이템 인벤토리의 DefaultItemSlots 활용
	if (DefaultItemSlots.Num() >= 5)
	{
		HelmetSlot = DefaultItemSlots[0];
		ArmorSlot = DefaultItemSlots[1];
		PantsSlot = DefaultItemSlots[2];
		GlovesSlot = DefaultItemSlots[3];
		BootsSlot = DefaultItemSlots[4];
	}
}

bool UPlayerEquipmentComponent::GetEquipmentSlotInfo(EEquipmentType EquipmentType, int32& OutSlotIndex, FInventorySlot*& OutTargetSlot)
{
	switch (EquipmentType)
	{
	case EEquipmentType::Helmet:
		OutSlotIndex = 0;
		OutTargetSlot = &HelmetSlot;
		return true;

	case EEquipmentType::Armor:
		OutSlotIndex = 1;
		OutTargetSlot = &ArmorSlot;
		return true;

	case EEquipmentType::Pants:
		OutSlotIndex = 2;
		OutTargetSlot = &PantsSlot;
		return true;

	case EEquipmentType::Gloves:
		OutSlotIndex = 3;
		OutTargetSlot = &GlovesSlot;
		return true;

	case EEquipmentType::Boots:
		OutSlotIndex = 4;
		OutTargetSlot = &BootsSlot;
		return true;

	default:
		OutSlotIndex = -1;
		OutTargetSlot = nullptr;
		return false;
	}
}

void UPlayerEquipmentComponent::RemoveExistingEquipment(FInventorySlot* TargetSlot)
{
	UDataAsset_RPGItemData* const OldItemData = TargetSlot->ItemDataAsset.LoadSynchronous();
	if (!IsValid(OldItemData)) return;
	

	UDataAsset_RPGItemData_Equipment* const OldEquipmentData = Cast<UDataAsset_RPGItemData_Equipment>(OldItemData);
	if (!IsValid(OldEquipmentData)) return;
	

	// 스탯 처리, 인벤토리 처리
	RemoveEquipmentStats(OldEquipmentData);
	ReturnEquipmentToInventory(*TargetSlot);
}

void UPlayerEquipmentComponent::SetEquipmentSlot(const FInventorySlot& ItemToSet, int32 SlotIndex, FInventorySlot* TargetSlot)
{
	if (!TargetSlot || SlotIndex < 0 || SlotIndex >= DefaultItemSlots.Num()) return;
	

	// 장비슬롯에 아이템 적용
	*TargetSlot = ItemToSet;
	TargetSlot->SlotIndex = SlotIndex;
	TargetSlot->InventoryRef = this;
	DefaultItemSlots[SlotIndex] = *TargetSlot;
}

void UPlayerEquipmentComponent::ApplyEquipmentStats(const UDataAsset_RPGItemData_Equipment* EquipmentData)
{
	if (!IsValid(EquipmentData)) return;
	

	const APlayerCharacterBase* Player = Cast<APlayerCharacterBase>(GetOwner());
	if (!IsValid(Player)) return;
	

	URPGAttributeSet* const AttributeSet = Player->GetRPGAttributeSet();
	if (!IsValid(AttributeSet)) return;
	

	//AttributeSet에 스탯 적용 
	AttributeSet->ApplyEquipmentStats(
		EquipmentData->MaxHp,
		EquipmentData->MaxMp,
		EquipmentData->AttackRate,
		EquipmentData->Defense
	);
}

void UPlayerEquipmentComponent::RemoveEquipmentStats(const UDataAsset_RPGItemData_Equipment* EquipmentData)
{
	if (!IsValid(EquipmentData)) return;
	

	const APlayerCharacterBase* Player = Cast<APlayerCharacterBase>(GetOwner());
	if (!IsValid(Player)) return;
	

	URPGAttributeSet* const AttributeSet = Player->GetRPGAttributeSet();
	if (!IsValid(AttributeSet)) return;
	

	//AttributeSet에 스탯 적용 
	AttributeSet->RemoveEquipmentStats(
		EquipmentData->MaxHp,
		EquipmentData->MaxMp,
		EquipmentData->AttackRate,
		EquipmentData->Defense
	);
}

void UPlayerEquipmentComponent::ReturnEquipmentToInventory(const FInventorySlot& EquipmentSlot)
{
	if (!IsValid(PlayerInventoryComponentRef)) return;
	

	PlayerInventoryComponentRef->AddItem(EquipmentSlot);
}

void UPlayerEquipmentComponent::ClearEquipmentSlot(int32 SlotIndex, FInventorySlot* TargetSlot)
{
	if (!TargetSlot || SlotIndex < 0 || SlotIndex >= DefaultItemSlots.Num()) return;
	
	*TargetSlot = FInventorySlot();
	TargetSlot->SlotIndex = SlotIndex;
	TargetSlot->InventoryRef = this;
	DefaultItemSlots[SlotIndex] = *TargetSlot;
}

void UPlayerEquipmentComponent::ApplyEquipmentItem(FInventorySlot ItemToSet)
{
	UDataAsset_RPGItemData* const ItemData = ItemToSet.ItemDataAsset.LoadSynchronous();
	if (!IsValid(ItemData)) return;
	

	UDataAsset_RPGItemData_Equipment* const EquipmentData = Cast<UDataAsset_RPGItemData_Equipment>(ItemData);
	if (!IsValid(EquipmentData)) return;
	

	int32 SlotIndex = -1;
	FInventorySlot* TargetSlot = nullptr;
	if (!GetEquipmentSlotInfo(EquipmentData->EquipmentType, SlotIndex, TargetSlot)) return;
	
	// Equip 로직처리
	if (TargetSlot->ItemDataAsset.IsValid())
	{
		RemoveExistingEquipment(TargetSlot);
	}
	SetEquipmentSlot(ItemToSet, SlotIndex, TargetSlot);
	ApplyEquipmentStats(EquipmentData);
	OnEquipmentSlotChangedDelegate.Broadcast();
}
void UPlayerEquipmentComponent::ApplyUnEquipmentItem(FInventorySlot ItemToSet)
{
	UDataAsset_RPGItemData* const ItemData = ItemToSet.ItemDataAsset.LoadSynchronous();
	if (!IsValid(ItemData)) return;
	

	UDataAsset_RPGItemData_Equipment* const EquipmentData = Cast<UDataAsset_RPGItemData_Equipment>(ItemData);
	if (!IsValid(EquipmentData)) return;
	
	int32 SlotIndex = -1;
	FInventorySlot* TargetSlot = nullptr;
	if (!GetEquipmentSlotInfo(EquipmentData->EquipmentType, SlotIndex, TargetSlot)) return;
	
	if (!TargetSlot->ItemDataAsset.IsValid()) return;

	// UnEquip 로직처리
	RemoveEquipmentStats(EquipmentData);
	ReturnEquipmentToInventory(*TargetSlot);
	ClearEquipmentSlot(SlotIndex, TargetSlot);
	OnEquipmentSlotChangedDelegate.Broadcast();
}

