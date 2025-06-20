// LJS


#include "DataAsset/DataAsset_RPGLevelData.h"

UDataAsset_RPGLevelData::UDataAsset_RPGLevelData(const FObjectInitializer& ObjectInitializer)
{
}

TSoftObjectPtr<UWorld> UDataAsset_RPGLevelData::GetGameLevelByTag(FGameplayTag GameplayTag) const
{
	for (const FLevelGameplayTag& GameLevel : GameLevelArray)
	{
		if(!GameLevel.IsValid()) continue;

		if (GameLevel.LevelTag == GameplayTag)
		{
			return GameLevel.Level;
		}
	}

	return TSoftObjectPtr<UWorld>();
}
