// LJS


#include "GameAbilitySystem/GamePlayCue/Player/RPGGC_Character_PlaySoundAtTarget.h"
#include "Kismet/GameplayStatics.h"

URPGGC_Character_PlaySoundAtTarget_AttackHit::URPGGC_Character_PlaySoundAtTarget_AttackHit()
{
}

bool URPGGC_Character_PlaySoundAtTarget_AttackHit::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, Target->GetActorLocation());
	
	return Super::OnExecute_Implementation(Target, Parameters);;
}
