// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameMode/RPGGameModeBase.h"
#include "DungeonGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSequenceFinished);

class ULevelSequence;
class AMonsterCharacter;

/**
 * ADungeonGameMode
 * 
 * 던전 게임 모드 - 던전 시네마 시스템, 몬스터 스폰 관리 등
 */
UCLASS()
class RPGPORTFOLIO_API ADungeonGameMode : public ARPGGameModeBase
{
	GENERATED_BODY()
public:
	//@ 던전 시네마 재생 시작
	void DungeonCinemaPlay();

	//@ 던전 시네마 완료 델리게이트 
	UPROPERTY(BlueprintAssignable)
	FOnSequenceFinished OnDungeonCinemaFinished;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	//@ 일반 몬스터 스폰 볼륨 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	FString SpawnVolume;

	//@ 보스 몬스터 스폰 볼륨
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	FString BossSpawnVolume;

	//@ 레벨 시퀀스 에셋 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	ULevelSequence* LevelSequenceAsset;

	//@ 게임 경과 시간
	UPROPERTY()
	float GameTime;

	//@ 몬스터 스폰 완료 플래그
	bool flagspawn;

	//@ 페이드 백 타이머 핸들
	FTimerHandle FadeBackTimerHandle;

	//@ 시네마 완료 시 호출되는 콜백 함수
	UFUNCTION()
	void OnCinemaFinished();

	//@ 보스 등장 처리
	UFUNCTION()
	void BossAppear();

	//@ 시네마 관련 UI, 연출 처리
	void StartCinema();
	void HidePlayerUI();
	void ShowPlayerUI();
	void StartCameraFadeInOut();

	
};
