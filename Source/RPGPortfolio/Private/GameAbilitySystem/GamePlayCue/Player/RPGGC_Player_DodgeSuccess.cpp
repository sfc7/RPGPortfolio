// LJS


#include "GameAbilitySystem/GamePlayCue/Player/RPGGC_Player_DodgeSuccess.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

URPGGC_Player_DodgeSuccess::URPGGC_Player_DodgeSuccess()
{
}

bool URPGGC_Player_DodgeSuccess::OnExecute_Implementation(AActor* Target,const FGameplayCueParameters& Parameters) const
{
	// 회피 성공 사운드 재생
	UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, Target->GetActorLocation());

	FVector SocketLocation = FVector::ZeroVector;
	// 회피 성공 이펙트를 해당 소켓에 생성
	if (Parameters.TargetAttachComponent->DoesSocketExist(SocketName))
	{
		SocketLocation = Parameters.TargetAttachComponent->GetSocketLocation(SocketName);	
	}
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this, DodgeEffect, SocketLocation, FRotator::ZeroRotator, FVector::OneVector,
		true,true, ENCPoolMethod::None, true);
	
	return Super::OnExecute_Implementation(Target, Parameters);
}
