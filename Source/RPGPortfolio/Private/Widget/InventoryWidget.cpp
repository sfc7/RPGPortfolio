// LJS


#include "Widget/InventoryWidget.h"
#include "Component/InventoryComponent.h"
#include "Character/Player/PlayerCharacter_Fighter.h"
#include "Widget/ItemSlotContainer.h"
#include "Component/Player/PlayerInventoryComponent.h"
#include "Components/TextBlock.h"

UInventoryWidget::UInventoryWidget()
{
}

void UInventoryWidget::OnPlayerGoldChanged(int32 NewGoldAmount)
{
	if (PlayerGold)
	{
		PlayerGold->SetText(FText::AsNumber(NewGoldAmount));
	}
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	APlayerCharacter_Fighter* PC = Cast<APlayerCharacter_Fighter>(GetOwningPlayerPawn());
	if (IsValid(PC))
	{
		// 아이템 슬롯 컨테이너에 인벤토리 참조 설정, 골드 설정
		ItemSlotContainer->SetInventoryRef(	PC->GetPlayerInventoryComponent());
		OnPlayerGoldChanged(PC->GetPlayerInventoryComponent()->GetPlayerGold());
	}

	ItemSlotContainer->GetInventoryRef()->OnGoldChangedDelegate.AddDynamic(this, &UInventoryWidget::OnPlayerGoldChanged);
}
