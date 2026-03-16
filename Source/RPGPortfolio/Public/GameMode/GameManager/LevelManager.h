// LJS

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LevelManager.generated.h"


class UDataAsset_RPGLevelData;
class URPGSaveGame;
/**
 * ULevelManager
 * 
 * 레벨, 세이브 관리 서브시스템
 */
UCLASS()
class RPGPORTFOLIO_API ULevelManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	ULevelManager();

	//@ 게임플레이 태그로 게임 레벨 가져오기
	UFUNCTION(BlueprintPure)
	TSoftObjectPtr<UWorld> GetGameLevelByTag(FGameplayTag GameplayTag) const;

	//@ SaveGame으로 데이터 저장
	UFUNCTION(BlueprintCallable)
	void SaveRPGGame();

	//@ SaveGame으로 데이터 로드
	UFUNCTION(BlueprintCallable)
	void LoadRPGGame();

	//@ 플레이어 인벤토리 데이터 저장
	UFUNCTION(BlueprintCallable)
	void SavePlayerInventoryData(URPGSaveGame* SaveGame);

	//@ 플레이어 인벤토리 데이터 로드
	UFUNCTION(BlueprintCallable)
	void LoadPlayerInventoryData(URPGSaveGame* SaveGame);

	//@ 플레이어 골드 저장 
	UFUNCTION(BlueprintCallable)
	void SavePlayerGold(URPGSaveGame* SaveGame);

	//@ 플레이어 골드 로드
	UFUNCTION(BlueprintCallable)
	void LoadPlayerGold(URPGSaveGame* SaveGame);

	//@ 플레이어 AttributeSet 저장
	UFUNCTION(BlueprintCallable)
	void SavePlayerAttribute(URPGSaveGame* SaveGame);

	//@ 플레이어 AttributeSet 로드
	UFUNCTION(BlueprintCallable)
	void LoadPlayerAttribute(URPGSaveGame* SaveGame);

	//@ 로딩 스크린 로드
	void LoadLoadingScreen(const FString& LevelName);
	//@ 로딩 성공 이후 스크린 중단
	void DestinationLoadingScreen (UWorld* LoadWorld);
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
private:
	//@ 레벨 데이터 에셋
	UPROPERTY()
	UDataAsset_RPGLevelData* RPGLevelData;
};
