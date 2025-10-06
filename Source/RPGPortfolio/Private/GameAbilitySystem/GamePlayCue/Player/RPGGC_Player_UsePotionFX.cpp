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

	const FVector SpawnEffectLocation = Target->GetActorLocation();
	
	// 물약 사용 사운드 재생
	UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, Target->GetActorLocation());

	// 물약 사용 이펙트를 해당 소켓에 재생
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
