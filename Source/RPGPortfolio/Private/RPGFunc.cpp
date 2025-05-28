// LJS


#include "RPGFunc.h"
#include "GenericTeamAgentInterface.h"

bool URPGFunc::CheckTargetTeamAgent(APawn* SourcePawn, APawn* TargetPawn)
{
	if (IsValid(SourcePawn) && IsValid(TargetPawn))
	{
		IGenericTeamAgentInterface* SourceTeamAgent = Cast<IGenericTeamAgentInterface>(SourcePawn->GetController());
		IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

		if (SourceTeamAgent && TargetTeamAgent)
		{
			return SourceTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
		}
	}

	return false;
}
