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
	
	UGameplayStatics::SpawnEmitterAtLocation(
		this, 
		BuffEffect,                  
		SpawnEffectLocation, 
		FRotator::ZeroRotator, 
		FVector::OneVector,          
		true                         
	);

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

	if (IsValid(LeftHandActiveEffect))
	{
		LeftHandActiveEffect->DestroyComponent();
	}

	if (IsValid(RightHandActiveBuffEffect))
	{
		RightHandActiveBuffEffect->DestroyComponent();
	}
	return true;
}
