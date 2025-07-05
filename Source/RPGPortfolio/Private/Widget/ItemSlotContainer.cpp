// LJS


#include "Widget/ItemSlotContainer.h"

#include "Component/Player/PlayerInventoryComponent.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "GameMode/GameManager/UIManager.h"
#include "Widget/ItemSlotMaster.h"
UItemSlotContainer::UItemSlotContainer(const FObjectInitializer& ObjectInitializer)
{
}

void UItemSlotContainer::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UItemSlotContainer::ResizeSlot()
{
	for (int32 Index = 0; Index < TotalSlots; Index++)
	{
		UItemSlotMaster* PlayerUI = CreateWidget<UItemSlotMaster>(GetWorld()->GetFirstPlayerController(), ItemSlotWidgetClass);
		PlayerUI->SetInventoryRef(InventoryRef);
		PlayerUI->SlotData.SlotIndex = Index;
		
		SlotWidgets.Add(PlayerUI);

		int32 Row = Index / SlotsPerRow;
		int32 LastColumn = Index % SlotsPerRow;
		
		UUniformGridSlot* CreateUniformGridSlot = GridForSlots->AddChildToUniformGrid(PlayerUI, Row , LastColumn);
		CreateUniformGridSlot->SetHorizontalAlignment(HAlign_Center);
		CreateUniformGridSlot->SetVerticalAlignment(VAlign_Center);
	}
}

void UItemSlotContainer::RefreshSlots()
{
	GridForSlots->ClearChildren();
	SlotWidgets.Empty();

	ResizeSlot();

	for (FInventorySlot ItemSlot : InventoryRef->ItemSlots)
	{
		UpdateSlotWidgetData(ItemSlot);
	}
}

void UItemSlotContainer::UpdateSlotWidgetData(FInventorySlot UpdateSlot)
{
	SlotWidgets[UpdateSlot.SlotIndex]->UpdateSlotData(UpdateSlot);
}

void UItemSlotContainer::SetInventoryRef(UPlayerInventoryComponent* InventoryRefToSet)
{
	InventoryRef = InventoryRefToSet;
	TotalSlots = InventoryRef->SlotAmounts;
	
	RefreshSlots();

	InventoryRef->OnInventorySlotChangedDelegate.AddDynamic(this, &UItemSlotContainer::UpdateSlotWidgetData);
}
