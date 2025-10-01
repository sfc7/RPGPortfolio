// LJS


#include "GameMode/DungeonGameMode.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameMode/GameManager/DungeonProgressManager.h"
#include "GameMode/GameManager/GeneralGameManager.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/RPGWidgetBase.h"
#include "WorldStatic/SpawningVolume/SpawningVolume.h"

void ADungeonGameMode::DungeonCinemaPlay()
{
	StartCameraFadeInOut();
	HidePlayerUI();
    
	FTimerHandle FadeTimer;
	GetWorld()->GetTimerManager().SetTimer(FadeTimer, [this]()
	{
		StartCinema();
	}, 1.0f, false);
}

void ADungeonGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ADungeonGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GameTime += DeltaTime;

	if (GameTime >= 5.f && !flagspawn)
	{
		if (ASpawningVolume* FoundVolume = GetGameInstance()->GetSubsystem<UGeneralGameManager>()->GetDungeonProgressManager()->FindSpawningVolumebyName(SpawnVolume))
		{
			GetGameInstance()->GetSubsystem<UGeneralGameManager>()->GetDungeonProgressManager()->SpawnMonster(*FoundVolume);
		}
		
		flagspawn = true;
	}
}

void ADungeonGameMode::OnCinemaFinished()
{
	// 시네마 완료 후 카메라 페이드 효과
	StartCameraFadeInOut();

	FTimerHandle BossTimer;
	GetWorld()->GetTimerManager().SetTimer(BossTimer, [this]()
	{
		ShowPlayerUI();
		BossAppear();
	}, 1.0f, false);
}

void ADungeonGameMode::HidePlayerUI()
{
	APlayerController* const PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(PC)) return;
    
	// 모든 위젯 찾기
	TArray<UUserWidget*> AllWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), AllWidgets, URPGWidgetBase::StaticClass());
    
	// 모든 위젯 숨김
	for (UUserWidget* const Widget : AllWidgets)
	{
		if (IsValid(Widget))
		{
			Widget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ADungeonGameMode::ShowPlayerUI()
{
	APlayerController* const PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(PC)) return;
    
	// 모든 위젯 찾기
	TArray<UUserWidget*> AllWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), AllWidgets, URPGWidgetBase::StaticClass());
    
	// 모든 위젯 표시
	for (UUserWidget* const Widget : AllWidgets)
	{
		if (IsValid(Widget))
		{
			Widget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ADungeonGameMode::StartCameraFadeInOut()
{
	APlayerController* const PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(PC)) return;
	
	APlayerCameraManager* const CameraManager = PC->PlayerCameraManager;
	if (!IsValid(CameraManager)) return;

	// 페이드 아웃 시작
	CameraManager->StartCameraFade(0.0f, 1.0f, 1.0f, FLinearColor::Black, false, true);
        
	// 페이드 인 타이머 설정
	FTimerHandle FadeInTimer;
	const float FadeInDelay = 1.2f;
	
	GetWorld()->GetTimerManager().SetTimer(FadeInTimer, [PC]()
	{
		if (!IsValid(PC)) return;
		
		APlayerCameraManager* const CameraManager = PC->PlayerCameraManager;
		if (!IsValid(CameraManager)) return;

		// 페이드 인 시작
		CameraManager->StartCameraFade(1.0f, 0.0f, 1.0f, FLinearColor::Black, false, true);
	}, FadeInDelay, false);
}

void ADungeonGameMode::BossAppear()
{
	UGeneralGameManager* const GeneralGameManager = GetGameInstance()->GetSubsystem<UGeneralGameManager>();
	if (!IsValid(GeneralGameManager)) return;

	UDungeonProgressManager* const DungeonProgressManager = GeneralGameManager->GetDungeonProgressManager();
	if (!IsValid(DungeonProgressManager)) return;

	ASpawningVolume* const FoundVolume = DungeonProgressManager->FindSpawningVolumebyName(BossSpawnVolume);
	if (IsValid(FoundVolume))
	{
		DungeonProgressManager->SpawnBossMonster(*FoundVolume);
	}
}

void ADungeonGameMode::StartCinema()
{
	if (!IsValid(LevelSequenceAsset)) return;

	// 시퀀스 재생 설정
	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	PlaybackSettings.bAutoPlay = true; 

	ALevelSequenceActor* OutActor = nullptr;

	// 레벨 시퀀스 플레이어 생성
	ULevelSequencePlayer* const SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(),
		LevelSequenceAsset,
		PlaybackSettings,
		OutActor
	);

	if (IsValid(SequencePlayer)) {
		// 완료 콜백 바인딩 및 재생 시작
		SequencePlayer->OnFinished.AddDynamic(this, &ADungeonGameMode::OnCinemaFinished);
		SequencePlayer->Play();
	}
}

