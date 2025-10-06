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
	// 패링 사운드 재생
	UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, Target->GetActorLocation());
	
	// 패링 이펙트의 위치와 회전 값을 캐릭터 입장에서 계산후 해당 소켓에 생성
	FVector SocketLocation;
	if (Parameters.TargetAttachComponent->DoesSocketExist(SocketName))
	{
		SocketLocation = Parameters.TargetAttachComponent->GetSocketLocation(SocketName);	
	}
	FVector SpawnEffectLocation = SocketLocation + Target->GetActorForwardVector() * 30.f;
	FRotator SpawnEffectRotation = UKismetMathLibrary::MakeRotFromX(Target->GetActorForwardVector());

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this, ParryingEffect, SpawnEffectLocation, SpawnEffectRotation, FVector::OneVector,
		true,true, ENCPoolMethod::None, true);

	
	return Super::OnExecute_Implementation(Target, Parameters);
}
