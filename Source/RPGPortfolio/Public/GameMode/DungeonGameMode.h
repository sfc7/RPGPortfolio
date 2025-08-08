// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameMode/RPGGameModeBase.h"
#include "DungeonGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSequenceFinished);

class ULevelSequence;
class AMonsterCharacter;

UCLASS()
class RPGPORTFOLIO_API ADungeonGameMode : public ARPGGameModeBase
{
	GENERATED_BODY()
public:
	void DungeonCinemaPlay();
	
	UPROPERTY(BlueprintAssignable)
	FOnSequenceFinished OnDungeonCinemaFinished;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	FString SpawnVolume;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	FString BossSpawnVolume;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	ULevelSequence* LevelSequenceAsset;
	
	UPROPERTY()
	float GameTime;
	
	bool flagspawn;

	FTimerHandle FadeBackTimerHandle;
	
	UFUNCTION()
	void OnCinemaFinished();

	UFUNCTION()
	void BossAppear();

	void StartCinema();
	void HidePlayerUI();
	void ShowPlayerUI();
	void StartCameraFadeInOut();

	
};
