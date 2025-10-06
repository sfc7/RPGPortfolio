// LJS


#include "GameAbilitySystem/GamePlayCue/Player/RPGGC_Player_ApplyBuffFX.h"

#include "NiagaraComponent.h"
#include "NiagaraComponentPoolMethodEnum.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

ARPGGC_Player_ApplyBuffFX::ARPGGC_Player_ApplyBuffFX()
{
	bAutoDestroyOnRemove = true;
	bAutoAttachToOwner = true;
}

bool ARPGGC_Player_ApplyBuffFX::WhileActive_Implementation(AActor* Target, const FGameplayCueParameters& Parameters)
{
	Super::WhileActive_Implementation(Target, Parameters);

	FVector SpawnEffectLocation = Target->GetActorLocation();

	UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, Target->GetActorLocation());

	// 버프 스킬 사용 시 지속적으로 나타낼 이펙트 생성
	UGameplayStatics::SpawnEmitterAtLocation(
		this, 
		BuffEffect,                  
		SpawnEffectLocation, 
		FRotator::ZeroRotator, 
		FVector::OneVector,          
		true                         
	);

	// 버프 스킬 사용 시 왼손에 지속적으로 나타낼 이펙트 생성
	LeftHandActiveEffect = UGameplayStatics::SpawnEmitterAttached(
		BuffActiveEffect,                              
		Parameters.TargetAttachComponent.Get(),       
		LeftHandSocketName,                           
		FVector::ZeroVector,                           
		FRotator::ZeroRotator,                        
		FVector::OneVector,                       
		EAttachLocation::KeepRelativeOffset         
	);

	if (IsValid(LeftHandActiveEffect))
	{
		LeftHandActiveEffect->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	}

	// 버프 스킬 사용 시 오른손에 지속적으로 나타낼 이펙트 생성
	RightHandActiveBuffEffect = UGameplayStatics::SpawnEmitterAttached(
		BuffActiveEffect,                               
		Parameters.TargetAttachComponent.Get(),         
		RightHandSocketName,                            
		FVector::ZeroVector,                            
		FRotator::ZeroRotator,                          
		FVector::OneVector,                             
		EAttachLocation::KeepRelativeOffset             
	);

	if (IsValid(RightHandActiveBuffEffect))
	{
		RightHandActiveBuffEffect->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	}
	return true;
}

bool ARPGGC_Player_ApplyBuffFX::OnRemove_Implementation(AActor* Target, const FGameplayCueParameters& Parameters)
{
	Super::OnRemove_Implementation(Target, Parameters);

	// 왼손 이펙트 제거
	if (IsValid(LeftHandActiveEffect))
	{
		LeftHandActiveEffect->DestroyComponent();
	}

	// 오른손 이펙트 제거
	if (IsValid(RightHandActiveBuffEffect))
	{
		RightHandActiveBuffEffect->DestroyComponent();
	}
	
	return true;
}
