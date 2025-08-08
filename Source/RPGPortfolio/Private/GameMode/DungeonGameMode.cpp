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
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;
    
	TArray<UUserWidget*> AllWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), AllWidgets, URPGWidgetBase::StaticClass());
    
	for (UUserWidget* Widget : AllWidgets)
	{
		Widget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ADungeonGameMode::ShowPlayerUI()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;
    
	TArray<UUserWidget*> AllWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), AllWidgets, URPGWidgetBase::StaticClass());
    
	for (UUserWidget* Widget : AllWidgets)
	{
		Widget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ADungeonGameMode::StartCameraFadeInOut()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC && PC->PlayerCameraManager)
	{
		PC->PlayerCameraManager->StartCameraFade(0.0f, 1.0f, 1.0f, FLinearColor::Black, false, true);
        
		FTimerHandle FadeInTimer;
		GetWorld()->GetTimerManager().SetTimer(FadeInTimer, [PC]()
		{
			if (PC && PC->PlayerCameraManager)
			{
				PC->PlayerCameraManager->StartCameraFade(1.0f, 0.0f, 1.0f, FLinearColor::Black, false, true);
			}
		}, 1.2f, false);
	}
}

void ADungeonGameMode::BossAppear()
{
	if (ASpawningVolume* FoundVolume = GetGameInstance()->GetSubsystem<UGeneralGameManager>()->GetDungeonProgressManager()->FindSpawningVolumebyName(BossSpawnVolume))
	{
		GetGameInstance()->GetSubsystem<UGeneralGameManager>()->GetDungeonProgressManager()->SpawnBossMonster(*FoundVolume);
	}
}

void ADungeonGameMode::StartCinema()
{
	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	PlaybackSettings.bAutoPlay = true; 

	ALevelSequenceActor* OutActor = nullptr;

	ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(),
		LevelSequenceAsset,
		PlaybackSettings,
		OutActor
	);

	if (SequencePlayer)
	{
		SequencePlayer->OnFinished.AddDynamic(this, &ADungeonGameMode::OnCinemaFinished);
		SequencePlayer->Play();
	}
}

