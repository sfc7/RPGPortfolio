// LJS


#include "GameMode/GameManager/LevelManager.h"

#include "DataAsset/DataAsset_RPGLevelData.h"

ULevelManager::ULevelManager()
{
	
}

void ULevelManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
    
	RPGLevelData = LoadObject<UDataAsset_RPGLevelData>(nullptr, 
		TEXT("/Game/MyProject/Widget/Data/DA_RPGLevelData.DA_RPGLevelData"));
}

void ULevelManager::Deinitialize()
{
	Super::Deinitialize();
}

TSoftObjectPtr<UWorld> ULevelManager::GetGameLevelByTag(FGameplayTag GameplayTag) const
{
	return RPGLevelData->GetGameLevelByTag(GameplayTag);
}
