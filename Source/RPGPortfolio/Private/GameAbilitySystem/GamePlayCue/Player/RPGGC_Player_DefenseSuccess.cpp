// LJS


#include "GameAbilitySystem/GamePlayCue/Player/RPGGC_Player_DefenseSuccess.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

URPGGC_Player_DefenseSuccess::URPGGC_Player_DefenseSuccess()
{
}

bool URPGGC_Player_DefenseSuccess::OnExecute_Implementation(AActor* Target,const FGameplayCueParameters& Parameters) const
{
	UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, Target->GetActorLocation());

	FVector SocketLocation = Parameters.TargetAttachComponent->GetSocketLocation(SocketName);
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this, DefenseEffect, SocketLocation, FRotator::ZeroRotator, FVector::OneVector,
		true,true, ENCPoolMethod::None, true);
	
	return Super::OnExecute_Implementation(Target, Parameters);
}
