// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "PlayerCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API APlayerCharacterController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
	APlayerCharacterController();
	
	virtual FGenericTeamId GetGenericTeamId() const override;

private:
	FGenericTeamId PlayerTeamID;
};
