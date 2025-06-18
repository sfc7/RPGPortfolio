// LJS

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGGameManager.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGameManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};
