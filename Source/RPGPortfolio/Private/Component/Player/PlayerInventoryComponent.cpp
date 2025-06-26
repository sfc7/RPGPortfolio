// LJS


#include "Component/Player/PlayerInventoryComponent.h"

#include "GameMode/GameManager/ItemManager.h"
#include "Kismet/KismetArrayLibrary.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameMode/GameManager/UIManager.h"

UPlayerInventoryComponent::UPlayerInventoryComponent()
{
}


void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	SetupSlots(SlotAmounts);
}

void UPlayerInventoryComponent::SetupSlots(int32 SlotAmountstoSetup)
{
	for (int Index = 0; Index<SlotAmountstoSetup; Index++)
	{
		FInventorySlot InventorySlot;
		InventorySlot.SlotIndex = Index;
		InventorySlot.InventoryRef = this;
		ItemSlots.Add(InventorySlot);
	}
}

bool UPlayerInventoryComponent::AddItem(FInventorySlot ItemToAdd)
{
	FInventorySlot FindInventorySlot;
	if (FindEmptySlot((FindInventorySlot)))
	{
		SetItem(FindInventorySlot, ItemToAdd);
		return true;
	}
	else
	{
		UE_LOG(LogTemp,Log, TEXT("Is Inventory Full"));
		return false;
	}
	
}

void UPlayerInventoryComponent::SetItem(FInventorySlot TargetSlot, FInventorySlot ItemToSet)
{
	int32 TargetIndex = TargetSlot.SlotIndex;	

	if (ItemSlots.IsValidIndex(TargetIndex))
	{
		ItemSlots[TargetIndex] = ItemToSet;
		ItemSlots[TargetIndex].SlotIndex = TargetIndex;
		ItemSlots[TargetIndex].InventoryRef = this;

		OnInventorySlotChangedDelegate.Broadcast(ItemToSet);
	}
}

bool UPlayerInventoryComponent::FindEmptySlot(FInventorySlot& OutEmptySlot)
{
	UE_LOG(LogTemp, Warning, TEXT("FindEmptySlot - this pointer: %p"), this);
	
	for (FInventorySlot& ItemSlot : ItemSlots)
	{
		UE_LOG(LogTemp,Log, TEXT("FindEmptySlot"));
		bool IsEmpty = GetWorld()->GetGameInstance()->GetSubsystem<UItemManager>()->IsInventorySlotEmpty(ItemSlot);
		if (IsEmpty)
		{
			OutEmptySlot = ItemSlot;
			return true;
		}
	}

	return false;
}



// void UPlayerInventoryComponent::SetCurrentInventoryWidget()
// {
// 	if (IsValid(InventoryWidget))
// 	{
// 		InventoryWidget->RemoveFromParent();
// 		InventoryWidget = nullptr;
// 	}
//
// 	TSoftClassPtr<UUserWidget> InventorySoftWidget = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->GetUIWidgetClass(EUICategory::InventoryUI);
// 	UClass* WidgetClass = InventorySoftWidget.LoadSynchronous();
// 	InventoryWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), WidgetClass);
//
// 	if (InventoryWidget)
// 	{
// 		InventoryWidget->AddToViewport();
// 	}
// }
