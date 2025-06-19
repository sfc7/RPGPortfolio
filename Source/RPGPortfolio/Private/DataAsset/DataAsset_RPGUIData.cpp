// LJS


#include "DataAsset/DataAsset_RPGUIData.h"

UDataAsset_RPGUIData::UDataAsset_RPGUIData(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

TSoftClassPtr<UUserWidget> UDataAsset_RPGUIData::GetUIWidgetClass(const EUICategory& UICategory) const
{
	switch (UICategory)
	{
	case EUICategory::GameOverUI:
		return GameOverUI.UIWidgetClass;
	default:
		return nullptr;
	}
}