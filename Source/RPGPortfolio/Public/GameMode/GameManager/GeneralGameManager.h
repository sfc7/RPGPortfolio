// LJS

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGStructTypes.h"
#include "GeneralGameManager.generated.h"

class ULevelManager;
class UUIManager;
class UDungeonProgressManager;
class UItemManager;
class UInteractManager;
class UQuestManager;
class UGASManager;
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

	UFUNCTION(BlueprintPure)
	UItemManager* GetItemManager() const;

	UFUNCTION(BlueprintPure)
	UInteractManager* GetInteractManager() const;

	UFUNCTION(BlueprintPure)
	UQuestManager* GetQuestManager() const;

	UFUNCTION(BlueprintPure)
	UGASManager* GetGASManager() const;
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};
