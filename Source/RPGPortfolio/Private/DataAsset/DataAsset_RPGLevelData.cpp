// LJS


#include "DataAsset/DataAsset_RPGLevelData.h"

UDataAsset_RPGLevelData::UDataAsset_RPGLevelData(const FObjectInitializer& ObjectInitializer)
{
}

TSoftObjectPtr<UWorld> UDataAsset_RPGLevelData::GetGameLevelByTag(FGameplayTag GameplayTag) const
{
	if (!GameplayTag.IsValid()) return TSoftObjectPtr<UWorld>();

	if (GameLevelArray.IsEmpty()) return TSoftObjectPtr<UWorld>();

	// 모든 게임 레벨에서 해당 태그 검색
	for (const FLevelGameplayTag& GameLevel : GameLevelArray)
	{
		if (!GameLevel.IsValid()) continue;

		// 태그 매칭 확인
		const bool bTagMatches = (GameLevel.LevelTag == GameplayTag);
		if (bTagMatches)
		{
			return GameLevel.Level;
		}
	}

	// 해당 태그의 레벨을 찾지 못함
	return TSoftObjectPtr<UWorld>();
}
