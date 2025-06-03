// LJS


#include "GameAbilitySystem/GamePlayCue/Player/RPGGC_Player_Defense.h"

ARPGGC_Player_Defense::ARPGGC_Player_Defense()
{
	bAutoDestroyOnRemove = true;
	bAutoAttachToOwner = true;
}

bool ARPGGC_Player_Defense::WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	return Super::WhileActive_Implementation(MyTarget, Parameters);
}

bool ARPGGC_Player_Defense::OnRemove_Implementation(AActor* Target, const FGameplayCueParameters& Parameters)
{
	return Super::OnRemove_Implementation(Target, Parameters);
}
