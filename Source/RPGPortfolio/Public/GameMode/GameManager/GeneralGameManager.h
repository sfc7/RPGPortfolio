// LJS

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GeneralGameManager.generated.h"

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
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};
