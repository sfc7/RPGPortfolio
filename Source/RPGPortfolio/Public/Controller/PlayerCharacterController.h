// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "PlayerCharacterController.generated.h"

/**
 * APlayerCharacterController
 * 
 * PlayerCharacter를 위한 Controller
 */
UCLASS()
class RPGPORTFOLIO_API APlayerCharacterController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
	APlayerCharacterController();

	// GAS, CrowdComponent등에 쓸 TeamId
	virtual FGenericTeamId GetGenericTeamId() const override;

private:
	// GAS, CrowdComponent등에 쓸 TeamId
	FGenericTeamId PlayerTeamID;
};
