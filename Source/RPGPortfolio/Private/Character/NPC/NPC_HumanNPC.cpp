// LJS


#include "Character/NPC/NPC_HumanNPC.h"

#include "Blueprint/UserWidget.h"
#include "Component/Player/QuestNPCComponent.h"
#include "GameMode/GameManager/UIManager.h"
#include "Widget/StoreWidget.h"
#include "Component/InventoryComponent.h"


ANPC_HumanNPC::ANPC_HumanNPC()
{
	NPCType = ENPCType::Conversational;
	StoreInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("StoreInventory"));
}

UUserWidget* ANPC_HumanNPC::GetStoreWidget()
{
	if (CachedStoreWidget && IsValid(CachedStoreWidget))
	{
		return CachedStoreWidget;
	}

	TSubclassOf<UUserWidget> StoreWidgetClass = GetGameInstance()->GetSubsystem<UUIManager>()->GetInteractWidgetClass(EInteractUICategory::Store);
	if (!IsValid(StoreWidgetClass))
	{
		return nullptr;
	}
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), StoreWidgetClass);
	UStoreWidget* StoreWidget = Cast<UStoreWidget>(Widget);
	
	if (!IsValid(StoreWidget)) return nullptr;
	StoreWidget->StoreInventoryRef = StoreInventory;
	
	CachedStoreWidget = StoreWidget;
	
	return StoreWidget;
}