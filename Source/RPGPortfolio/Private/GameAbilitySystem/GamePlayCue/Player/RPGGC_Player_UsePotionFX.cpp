// LJS


#include "GameAbilitySystem/GamePlayCue/Player/RPGGC_Player_UsePotionFX.h"
#include "Kismet/GameplayStatics.h"

ARPGGC_Player_UsePotionFX::ARPGGC_Player_UsePotionFX()
{
	bAutoDestroyOnRemove = true;
	bAutoAttachToOwner = true;
}

bool ARPGGC_Player_UsePotionFX::WhileActive_Implementation(AActor* Target, const FGameplayCueParameters& Parameters)
{
	Super::WhileActive_Implementation(Target, Parameters);

	FVector SpawnEffectLocation = Target->GetActorLocation();

	UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, Target->GetActorLocation());

	UGameplayStatics::SpawnEmitterAtLocation(
		this, 
		UseItemActiveEffect,                  
		SpawnEffectLocation, 
		FRotator::ZeroRotator, 
		FVector::OneVector,          
		true                         
	);

	return true;
}

bool ARPGGC_Player_UsePotionFX::OnRemove_Implementation(AActor* Target, const FGameplayCueParameters& Parameters)
{
	Super::OnRemove_Implementation(Target, Parameters);
	
	return true;
}
