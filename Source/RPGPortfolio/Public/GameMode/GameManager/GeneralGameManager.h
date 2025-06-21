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
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UDungeonProgressManager* GetDungeonProgressManager() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UUIManager* GetUIManager() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ULevelManager* GetLevelManager() const; 

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void ToggleInputMode(const UObject* WorldContextObject, ERPGInputMode InputMode);
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};
