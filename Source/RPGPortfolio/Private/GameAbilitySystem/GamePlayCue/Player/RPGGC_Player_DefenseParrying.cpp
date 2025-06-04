// LJS


#include "GameAbilitySystem/GamePlayCue/Player/RPGGC_Player_DefenseParrying.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

URPGGC_Player_DefenseParrying::URPGGC_Player_DefenseParrying()
{
}

bool URPGGC_Player_DefenseParrying::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, Target->GetActorLocation());

	FVector SocketLocation = Parameters.TargetAttachComponent->GetSocketLocation(SocketName);
	FVector SpawnEffectLocation = SocketLocation + Target->GetActorForwardVector() * 30.f;
	FRotator SpawnEffectRotation = UKismetMathLibrary::MakeRotFromX(Target->GetActorForwardVector());

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this, ParryingEffect, SpawnEffectLocation, SpawnEffectRotation, FVector::OneVector,
		true,true, ENCPoolMethod::None, true);

	
	return Super::OnExecute_Implementation(Target, Parameters);
}
