// LJS


#include "Widget/ItemSlotContainer.h"

#include "Component/InventoryComponent.h"
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
	// 총 슬롯 개수만큼 슬롯 위젯 생성
	for (int32 Index = 0; Index < TotalSlots; Index++)
	{
		UItemSlotMaster* PlayerUI = CreateWidget<UItemSlotMaster>(GetWorld()->GetFirstPlayerController(), ItemSlotWidgetClass);
		PlayerUI->SetInventoryRef(InventoryRef);
		PlayerUI->SlotData.SlotIndex = Index;
		
		SlotWidgets.Add(PlayerUI);

		// 그리드 위치 계산 후 추가
		int32 Row = Index / SlotsPerRow;
		int32 LastColumn = Index % SlotsPerRow;
		
		UUniformGridSlot* CreateUniformGridSlot = GridForSlots->AddChildToUniformGrid(PlayerUI, Row , LastColumn);
		CreateUniformGridSlot->SetHorizontalAlignment(HAlign_Center);
		CreateUniformGridSlot->SetVerticalAlignment(VAlign_Center);
	}
}

void UItemSlotContainer::RefreshSlots()
{
	// 기존 슬롯 정리
	GridForSlots->ClearChildren();
	SlotWidgets.Empty();

	ResizeSlot();

	// 현재 아이템 슬롯들 업데이트
	for (FInventorySlot ItemSlot : InventoryRef->GetCurrentItemSlots())
	{
		UpdateSlotWidgetData(ItemSlot);
	}
}

void UItemSlotContainer::UpdateSlotWidgetData(FInventorySlot UpdateSlot)
{
	SlotWidgets[UpdateSlot.SlotIndex]->UpdateSlotData(UpdateSlot);
}

void UItemSlotContainer::SetInventoryRef(UInventoryComponent* InventoryRefToSet)
{
	InventoryRef = InventoryRefToSet;
	TotalSlots = InventoryRef->SlotAmounts;
	
	RefreshSlots();

	InventoryRef->OnInventorySlotChangedDelegate.AddDynamic(this, &UItemSlotContainer::UpdateSlotWidgetData);
}
