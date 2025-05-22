// LJS


#include "GameAbilitySystem/GamePlayCue/Player/RPGGC_Player_Sound_AttackHit.h"

#include "Kismet/GameplayStatics.h"

URPGGC_Player_Sound_AttackHit::URPGGC_Player_Sound_AttackHit()
{
}

bool URPGGC_Player_Sound_AttackHit::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, Target->GetActorLocation());
	
	return Super::OnExecute_Implementation(Target, Parameters);;
}
