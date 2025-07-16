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
		bFirstTimeLoadIn = false;
	}
}

void URPGGameInstance::OnDestinationWorldLoaded(UWorld* LoadedWorld)
{
	// 플레이어가 완전히 초기화될 때까지 잠시 대기
	FTimerHandle TimerHandle;
	LoadedWorld->GetTimerManager().SetTimer(TimerHandle, 
		[this]()
		{
			if (ULevelManager* LevelManager = GetSubsystem<ULevelManager>())
			{
				LevelManager->LoadRPGGame();
			}
		}, 
		0.3f, false); // 0.3초 후 로드
	
	GetMoviePlayer()->StopMovie();
}
