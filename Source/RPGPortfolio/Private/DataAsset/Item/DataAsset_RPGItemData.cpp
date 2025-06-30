// LJS


#include "DataAsset/Item/DataAsset_RPGItemData.h"

UDataAsset_RPGItemData::UDataAsset_RPGItemData(const FObjectInitializer& ObjectInitializer)
{
}

bool UDataAsset_RPGItemData::IsStackable()
{
	return StackSize > 1;
}
