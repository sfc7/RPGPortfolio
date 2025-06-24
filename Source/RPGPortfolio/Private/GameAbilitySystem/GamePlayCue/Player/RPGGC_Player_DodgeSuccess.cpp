// LJS


#include "GameAbilitySystem/GamePlayCue/Player/RPGGC_Player_DodgeSuccess.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

URPGGC_Player_DodgeSuccess::URPGGC_Player_DodgeSuccess()
{
}

bool URPGGC_Player_DodgeSuccess::OnExecute_Implementation(AActor* Target,const FGameplayCueParameters& Parameters) const
{
	UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, Target->GetActorLocation());

	FVector SocketLocation = Parameters.TargetAttachComponent->GetSocketLocation(SocketName);
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this, DodgeEffect, SocketLocation, FRotator::ZeroRotator, FVector::OneVector,
		true,true, ENCPoolMethod::None, true);
	
	return Super::OnExecute_Implementation(Target, Parameters);
}
