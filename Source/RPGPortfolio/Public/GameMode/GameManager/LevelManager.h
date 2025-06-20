// LJS

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LevelManager.generated.h"


class UDataAsset_RPGLevelData;
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
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
private:
	UPROPERTY()
	UDataAsset_RPGLevelData* RPGLevelData;
};
