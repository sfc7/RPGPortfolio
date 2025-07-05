// LJS

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LevelManager.generated.h"


class UDataAsset_RPGLevelData;
class URPGSaveGame;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API ULevelManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	ULevelManager();

	UFUNCTION(BlueprintPure)
	TSoftObjectPtr<UWorld> GetGameLevelByTag(FGameplayTag GameplayTag) const;

	UFUNCTION(BlueprintCallable)
	void SaveRPGGame();

	UFUNCTION(BlueprintCallable)
	void LoadRPGGame();

	UFUNCTION(BlueprintCallable)
	void SavePlayerInventoryData(URPGSaveGame* SaveGame);

	UFUNCTION(BlueprintCallable)
	void LoadPlayerInventoryData(URPGSaveGame* SaveGame);

	UFUNCTION(BlueprintCallable)
	void SavePlayerGold(URPGSaveGame* SaveGame);

	UFUNCTION(BlueprintCallable)
	void LoadPlayerGold(URPGSaveGame* SaveGame);

	UFUNCTION(BlueprintCallable)
	void SavePlayerAttribute(URPGSaveGame* SaveGame);

	UFUNCTION(BlueprintCallable)
	void LoadPlayerAttribute(URPGSaveGame* SaveGame);

	
	void LoadLoadingScreen(const FString& LevelName);
	void DestinationLoadingScreen(UWorld* LoadWorld);
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
private:
	UPROPERTY()
	UDataAsset_RPGLevelData* RPGLevelData;
};
