// LJS


#include "GameMode/RPGGameInstance.h"

#include "MoviePlayer.h"
#include "GameMode/GameManager/LevelManager.h"

class ULevelManager;

void URPGGameInstance::Init()
{
	Super::Init();

	// 맵 로딩 전후 델리게이트 바인딩
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this,&ThisClass::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this,&ThisClass::OnDestinationWorldLoaded);
}

void URPGGameInstance::OnPreLoadMap(const FString& MapName)
{
	// 로딩 스크린 속성 설정
	FLoadingScreenAttributes LoadingScreenAttributes;
	LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
	LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 2.f;
	LoadingScreenAttributes.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);

	// 레벨 매니저를 통한 게임 저장
	ULevelManager* const LevelManager = GetSubsystem<ULevelManager>();
	if (IsValid(LevelManager))
	{
		LevelManager->SaveRPGGame();
		bFirstTimeLoadIn = false;
	}
}

void URPGGameInstance::OnDestinationWorldLoaded(UWorld* LoadedWorld)
{
	if (!IsValid(LoadedWorld)) return;

	// 플레이어 초기화 대기 타이머 설정
	FTimerHandle TimerHandle;
	
	LoadedWorld->GetTimerManager().SetTimer(TimerHandle, 
		[this]()
		{
			// 레벨 매니저를 통한 게임 로드
			ULevelManager* const LevelManager = GetSubsystem<ULevelManager>();
			if (IsValid(LevelManager))
			{
				LevelManager->LoadRPGGame();
			}
		}, 
		DelayTime, false);
	
	// 로딩 스크린 종료
	GetMoviePlayer()->StopMovie();
}
