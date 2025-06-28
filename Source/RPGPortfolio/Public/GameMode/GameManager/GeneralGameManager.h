// LJS

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGStructTypes.h"
#include "GeneralGameManager.generated.h"

class ULevelManager;
class UUIManager;
class UDungeonProgressManager;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UGeneralGameManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure)
	UDungeonProgressManager* GetDungeonProgressManager() const;
	
	UFUNCTION(BlueprintPure)
	UUIManager* GetUIManager() const;

	UFUNCTION(BlueprintPure)
	ULevelManager* GetLevelManager() const; 
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};
