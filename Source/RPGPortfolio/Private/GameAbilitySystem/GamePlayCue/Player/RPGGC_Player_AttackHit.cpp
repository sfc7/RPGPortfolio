// LJS


#include "GameAbilitySystem/GamePlayCue/Player/RPGGC_Player_AttackHit.h"

#include "NiagaraComponentPoolMethodEnum.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"

URPGGC_Player_AttackHit::URPGGC_Player_AttackHit()
{
}

bool URPGGC_Player_AttackHit::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	FVector SpawnEffectLocation = Parameters.Location;
	FRotator SpawnEffectRotation = UKismetMathLibrary::MakeRotFromX(Parameters.Normal);
		
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this, AttackHitEffect, SpawnEffectLocation, SpawnEffectRotation, FVector::OneVector,
		true, true, ENCPoolMethod::None, true);

	return Super::OnExecute_Implementation(Target, Parameters);
}
