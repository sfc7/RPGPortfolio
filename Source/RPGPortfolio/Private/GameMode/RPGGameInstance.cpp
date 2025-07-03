// LJS


#include "GameMode/RPGGameInstance.h"

#include "MoviePlayer.h"
#include "GameMode/GameManager/LevelManager.h"

class ULevelManager;

void URPGGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this,&ThisClass::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this,&ThisClass::OnDestinationWorldLoaded);
}

void URPGGameInstance::OnPreLoadMap(const FString& MapName)
{
	FLoadingScreenAttributes LoadingScreenAttributes;
	LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
	LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 2.f;
	LoadingScreenAttributes.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);

	if (ULevelManager* LevelManager = GetSubsystem<ULevelManager>())
	{
		LevelManager->SaveRPGGame();
	}
}

void URPGGameInstance::OnDestinationWorldLoaded(UWorld* LoadedWorld)
{
	if (ULevelManager* LevelManager = GetSubsystem<ULevelManager>())
	{
		LevelManager->LoadRPGGame();
	}
	
	GetMoviePlayer()->StopMovie();
}
