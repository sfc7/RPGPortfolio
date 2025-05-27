// LJS


#include "Controller/PlayerCharacterController.h"

APlayerCharacterController::APlayerCharacterController()
{
	PlayerTeamID = FGenericTeamId(0);
}

FGenericTeamId APlayerCharacterController::GetGenericTeamId() const
{
	return PlayerTeamID;
}
