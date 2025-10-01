// LJS


#include "Widget/ItemSlotMaster.h"

#include "Character/NPC/NPC_HumanNPC.h"
#include "Character/Player/PlayerCharacter_Fighter.h"
#include "Components/Image.h"
#include "GameMode/GameManager/ItemManager.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/SizeBox.h"
#include "DataAsset/Item/DataAsset_RPGItemData.h"
#include "Widget/ItemInfomation.h"
#include "Component/InventoryComponent.h"
#include "Widget/StoreWidget.h"
#include "Components/Button.h"
#include "Component/Player/PlayerEquipmentComponent.h"
#include "Component/Player/PlayerInventoryComponent.h"
#include "DataAsset/Item/DataAsset_RPGItemData_Potion.h"

UItemSlotMaster::UItemSlotMaster(const FObjectInitializer& ObjectInitializer)
{
	
}

void UItemSlotMaster::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (Button)
	{
		Button->OnPressed.AddDynamic(this, &UItemSlotMaster::OnItemButtonClickedInStore);
	}
}

void UItemSlotMaster::UpdateSlotData(FInventorySlot UpdateSlotData)
{
	SlotData = UpdateSlotData;
	SlotHasItem = GetGameInstance()->GetSubsystem<UItemManager>()->IsInventorySlotNotEmpty(SlotData);

	SetWidgetVisibility(InfoLayer, SlotHasItem);

	// 아이템이 있는 경우 UI 업데이트
	if (SlotHasItem)
	{
		UDataAsset_RPGItemData* ItemDataAssetObject = SlotData.ItemDataAsset.LoadSynchronous();
		IconImage->SetBrushFromSoftTexture(ItemDataAssetObject->ItemIcon);
		QuantityText->SetText(FText::AsNumber(SlotData.Quantity));

		// 스택 가능 여부에 따른 수량 레이어 가시성
		bool VisibleFlag = ItemDataAssetObject->IsStackable();
		SetWidgetVisibility(QuantityLayer, VisibleFlag);

		// 툴팁 위젯 업데이트
		UItemInfomation* TooltipWidget = Cast<UItemInfomation>(GetToolTip());
		if (TooltipWidget)
		{
			TooltipWidget->UpdateSlotData(SlotData);
		}
	}
}

void UItemSlotMaster::SetWidgetVisibility(UWidget* Target, bool IsVisible)
{
	if (IsVisible) 
	{
		Target->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Target->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UItemSlotMaster::SetInventoryRef(UInventoryComponent* InventoryReftoSet)
{
	InventoryRef = InventoryReftoSet;
}

void UItemSlotMaster::SetSlotSizeBox(float Size)
{
	RPGSizeBox->SetWidthOverride(Size);
	RPGSizeBox->SetHeightOverride(Size);
}

bool UItemSlotMaster::CheckInventoryOwnerAndNPCType()
{
	if (!IsValid(InventoryRef)) return false;

	AActor* Owner = InventoryRef->GetOwner();
	if (!Owner) return false;
	
	if (ANPC_HumanNPC* HumanNPC = Cast<ANPC_HumanNPC>(Owner))
	{
		ENPCType NPCType = HumanNPC->GetNPCType();

		if (NPCType == ENPCType::Store)
		{
			return true;
		}
	}

	return false;
}

bool UItemSlotMaster::CheckIsPlayerInventory()
{
	if (!InventoryRef) return false;
	
	AActor* Owner = InventoryRef->GetOwner();
	if (!Owner) return false;
	    
	if (APlayerCharacter_Fighter* PlayerCharacter = Cast<APlayerCharacter_Fighter>(Owner))
	{
		return true;
	}

	return false;
}

bool UItemSlotMaster::CheckIsStoreInventory()
{
	if (!InventoryRef) return false;
	
	AActor* Owner = InventoryRef->GetOwner();
	if (!Owner) return false;
	
	if (ANPC_HumanNPC* HumanNPC = Cast<ANPC_HumanNPC>(Owner))
	{
		ENPCType NPCType = HumanNPC->GetNPCType();
        
		if (NPCType == ENPCType::Store)
		{
			return true;
		}
	}

	return false;
}

bool UItemSlotMaster::CheckDropTargetIsStoreInventory(UItemSlotMaster* TargetSlot)
{
	if (!IsValid(TargetSlot)) return false;
	
	return TargetSlot->CheckIsStoreInventory();
}

bool UItemSlotMaster::TrySellItem()
{
	if (!CheckIsPlayerInventory()) return false;
	
	if (!SlotHasItem || !SlotData.ItemDataAsset.IsValid()) return false;
	
	if (!IsValid(InventoryRef)) return false;
	
	return InventoryRef->TrySellItem(SlotData);
}

void UItemSlotMaster::OnItemButtonClickedInStore()
{
	if (!CheckInventoryOwnerAndNPCType()) return;
	

	if (!IsValid(InventoryRef))
	{
		return;
	}

	ANPC_HumanNPC* StoreNPC = Cast<ANPC_HumanNPC>(InventoryRef->GetOwner());
	if (!IsValid(StoreNPC)) return;
	
	
	UStoreWidget* StoreWidget = Cast<UStoreWidget>(StoreNPC->GetStoreWidget());
	if (StoreWidget)
	{
		StoreWidget->OnStoreItemPurchase(this);
	}
}

void UItemSlotMaster::OnItemDoubleClicked()
{
	APlayerCharacterBase* PlayerCharacter = Cast<APlayerCharacterBase>(InventoryRef->GetOwner());
	if (!IsValid(PlayerCharacter)) return;
	
	
	if (SlotData.ItemDataAsset->ItemType == EItemType::Equipment)
	{
		if (InventoryRef->GetInventoryType() == EInventoryType::PlayerInventory)
		{
			InventoryRef->HandleSlotDoubleClick(SlotData);
		}
		else if (InventoryRef->GetInventoryType() == EInventoryType::Equipment)
		{
			InventoryRef->UnEquipItem(SlotData);
		}
	}
}