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

	if (SlotHasItem)
	{
		UDataAsset_RPGItemData* ItemDataAssetObject = SlotData.ItemDataAsset.LoadSynchronous();
		IconImage->SetBrushFromSoftTexture(ItemDataAssetObject->ItemIcon);
		QuantityText->SetText(FText::AsNumber(SlotData.Quantity));

		bool VisibleFlag = ItemDataAssetObject->IsStackable();
		SetWidgetVisibility(QuantityLayer, VisibleFlag);

		UItemInfomation* TooltipWidget = Cast<UItemInfomation>(GetToolTip());
		if (TooltipWidget)
		{
			TooltipWidget->UpdateSlotData(SlotData);
		}

		if (Cast<UDefaultInventoryStrategy>(InventoryRef->CurrentInventoryStrategy.GetObject()))
		{
			PlusInfoText->SetText(FText::GetEmpty());
			PlusInfoText2->SetText(FText::GetEmpty());
		}
		else if (Cast<UEquipmentInventoryStrategy>(InventoryRef->CurrentInventoryStrategy.GetObject()))
		{
			UDataAsset_RPGItemData_Equipment* Equipment = Cast<UDataAsset_RPGItemData_Equipment>(ItemDataAssetObject);
			
			PlusInfoText->SetText(FText::Format(FText::FromString(TEXT("+{0}")), Equipment->AttackRate));
			PlusInfoText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));

			PlusInfoText2->SetText(FText::Format(FText::FromString(TEXT("+{0}")), Equipment->Defense));
			PlusInfoText2->SetColorAndOpacity(FSlateColor(FLinearColor::Blue));
		}
		else if (Cast<UPotionInventoryStrategy>(InventoryRef->CurrentInventoryStrategy.GetObject()))
		{
			UDataAsset_RPGItemData_Potion* Potion = Cast<UDataAsset_RPGItemData_Potion>(ItemDataAssetObject);
			
			PlusInfoText->SetText(FText::Format(FText::FromString(TEXT("+{0}")), Potion->HealAmount));
			PlusInfoText->SetColorAndOpacity(FSlateColor(FLinearColor::Green));

			PlusInfoText2->SetText(FText::GetEmpty());
		}
		else if (Cast<UMaterialInventoryStrategy>(InventoryRef->CurrentInventoryStrategy.GetObject()))
		{
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
	if (!InventoryRef)
	{
		return false;
	}

	AActor* Owner = InventoryRef->GetOwner();
	if (!Owner)
	{
		return false;
	}
	
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
	if (!InventoryRef)
	{
		return false;
	}

	AActor* Owner = InventoryRef->GetOwner();
	if (!Owner)
	{
		return false;
	}
    
	if (APlayerCharacter_Fighter* PlayerCharacter = Cast<APlayerCharacter_Fighter>(Owner))
	{
		return true;
	}

	return false;
}

bool UItemSlotMaster::CheckIsStoreInventory()
{
	if (!InventoryRef)
	{
		return false;
	}

	AActor* Owner = InventoryRef->GetOwner();
	if (!Owner)
	{
		return false;
	}
    
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
	if (!TargetSlot)
	{
		return false;
	}
    
	return TargetSlot->CheckIsStoreInventory();
}

bool UItemSlotMaster::TryPurchaseItem()
{
	 if (!CheckInventoryOwnerAndNPCType())
    {
        return false;
    }

    if (!SlotHasItem || !SlotData.ItemDataAsset.IsValid())
    {
        return false;
    }

    APlayerCharacter_Fighter* Player = Cast<APlayerCharacter_Fighter>(GetOwningPlayerPawn());
    if (!Player || !Player->GetPlayerInventoryComponent())
    {
        return false;
    }

    UInventoryComponent* PlayerInventory = Player->GetPlayerInventoryComponent();
	
    UDataAsset_RPGItemData* ItemData = SlotData.ItemDataAsset.LoadSynchronous();
    if (!ItemData)
    {
        return false;
    }

    int32 ItemPrice = ItemData->GoldValue;
    int32 PlayerGold = PlayerInventory->GetPlayerGold();
    
    if (PlayerGold < ItemPrice)
    {
        return false;
    }
	
    FInventorySlot ItemToBuy = SlotData;
    ItemToBuy.Quantity = 1;
    ItemToBuy.InventoryRef = PlayerInventory;

    bool bItemAdded = PlayerInventory->AddItem(ItemToBuy);
    if (!bItemAdded)
    {
        return false;
    }

    PlayerInventory->SetGold(-ItemPrice); 
    
    if (SlotData.Quantity > 1)
    {
        FInventorySlot ModifiedSlot = SlotData;
        InventoryRef->SetQuantityAtSlot(ModifiedSlot, SlotData.Quantity - 1);
    }
    else
    {
        InventoryRef->RemoveItemToIndex(SlotData.SlotIndex);
    }
    
    return true;
}

bool UItemSlotMaster::CanPurchaseItem()
{
	if (!CheckInventoryOwnerAndNPCType())
	{
		return false;
	}

	if (!SlotHasItem || !SlotData.ItemDataAsset.IsValid())
	{
		return false;
	}

	APlayerCharacter_Fighter* Player = Cast<APlayerCharacter_Fighter>(GetOwningPlayerPawn());
	if (!Player || !Player->GetPlayerInventoryComponent())
	{
		return false;
	}

	UDataAsset_RPGItemData* ItemData = SlotData.ItemDataAsset.LoadSynchronous();
	if (!ItemData)
	{
		return false;
	}

	int32 ItemPrice = ItemData->GoldValue;
	int32 PlayerGold = Player->GetPlayerInventoryComponent()->GetPlayerGold();
    
	return PlayerGold >= ItemPrice;
}

bool UItemSlotMaster::TrySellItem()
{
	if (!CheckIsPlayerInventory())
	{
		return false;
	}

	if (!SlotHasItem || !SlotData.ItemDataAsset.IsValid())
	{
		return false;
	}

	APlayerCharacter_Fighter* Player = Cast<APlayerCharacter_Fighter>(GetOwningPlayerPawn());
	if (!Player || !Player->GetPlayerInventoryComponent())
	{
		return false;
	}

	UInventoryComponent* PlayerInventory = Player->GetPlayerInventoryComponent();
    
	UDataAsset_RPGItemData* ItemData = SlotData.ItemDataAsset.LoadSynchronous();
	if (!ItemData)
	{
		return false;
	}

	int32 SellPrice = ItemData->GoldValue;
    
	PlayerInventory->SetGold(SellPrice);
    
	if (SlotData.Quantity > 1)
	{
		FInventorySlot ModifiedSlot = SlotData;
		InventoryRef->SetQuantityAtSlot(ModifiedSlot, SlotData.Quantity - 1);
	}
	else
	{
		InventoryRef->RemoveItemToIndex(SlotData.SlotIndex);
	}
    
	return true;
}

void UItemSlotMaster::OnItemButtonClickedInStore()
{
	if (!CheckInventoryOwnerAndNPCType())
	{
		return;
	}

	if (!InventoryRef)
	{
		return;
	}

	ANPC_HumanNPC* StoreNPC = Cast<ANPC_HumanNPC>(InventoryRef->GetOwner());
	if (!StoreNPC)
	{
		return;
	}

	
	UStoreWidget* StoreWidget = Cast<UStoreWidget>(StoreNPC->GetStoreWidget());
	if (StoreWidget)
	{
		StoreWidget->OnStoreItemPurchase(this);
	}
}

void UItemSlotMaster::OnEquipmentItemButtonDoubleClicked()
{
	if (SlotData.ItemDataAsset->ItemType == EItemType::Equipment)
	{
		if (InventoryRef->GetInventoryType() == EInventoryType::PlayerInventory)
		{
			APlayerCharacterBase* PlayerCharacter = Cast<APlayerCharacterBase>(InventoryRef->GetOwner());
			if (!PlayerCharacter)
			{
				return;
			}

			InventoryRef->EquipItem(SlotData);
		}
		else if (InventoryRef->GetInventoryType() == EInventoryType::Equipment)
		{
			APlayerCharacterBase* PlayerCharacter = Cast<APlayerCharacterBase>(InventoryRef->GetOwner());
			if (!PlayerCharacter)
			{
				return;
			}

			InventoryRef->UnEquipItem(SlotData);
		}
	}
}