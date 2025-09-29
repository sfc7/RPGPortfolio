// LJS


#include "Character/NPC/NPC_Storage.h"

#include "Blueprint/UserWidget.h"
#include "GameMode/GameManager/UIManager.h"
#include "Widget/StorageWidget.h"
#include "Component/InventoryComponent.h"

ANPC_Storage::ANPC_Storage()
{
	NPCType = ENPCType::Storage;
	StorageInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("StorageInventory"));
}

UUserWidget* ANPC_Storage::GetStorageWidget()
{
	TSubclassOf<UUserWidget> StorageWidgetClass = GetGameInstance()->GetSubsystem<UUIManager>()->GetInteractWidgetClass(EInteractUICategory::Storage);
	if (!IsValid(StorageWidgetClass)) return nullptr;
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), StorageWidgetClass);
	if (!IsValid(Widget)) return nullptr;
	
	UStorageWidget* StorageWidget = Cast<UStorageWidget>(Widget);
	if (!IsValid(StorageWidget)) return nullptr;

	
	StorageWidget->StorageInventoryRef = StorageInventory;
	
	return StorageWidget;
}
