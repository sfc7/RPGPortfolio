// LJS


#include "GameMode/DungeonGameMode.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "GameMode/GameManager/DungeonProgressManager.h"
#include "GameMode/GameManager/GeneralGameManager.h"
#include "WorldStatic/SpawningVolume/SpawningVolume.h"

void ADungeonGameMode::DungeonCinemaPlay()
{
	if (!IsValid(LevelSequenceAsset))
	{
		return;
	}

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
		SequencePlayer->OnFinished.AddDynamic(this, &ADungeonGameMode::HandleSequenceFinished);
		SequencePlayer->OnFinished.AddDynamic(this, &ADungeonGameMode::BossAppear);
		SequencePlayer->Play();
	}
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

void ADungeonGameMode::HandleSequenceFinished()
{
	OnDungeonCinemaFinished.Broadcast();
}

void ADungeonGameMode::BossAppear()
{
	if (ASpawningVolume* FoundVolume = GetGameInstance()->GetSubsystem<UGeneralGameManager>()->GetDungeonProgressManager()->FindSpawningVolumebyName(BossSpawnVolume))
	{
		GetGameInstance()->GetSubsystem<UGeneralGameManager>()->GetDungeonProgressManager()->SpawnBossMonster(*FoundVolume);
	}
}
