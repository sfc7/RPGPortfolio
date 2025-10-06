// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "RPGGC_Player_UsePotionFX.generated.h"

class UNiagaraSystem;
class UParticleSystem;
/**
 * URPGGC_Player_DodgeSuccess
 * 
 * 물약 사용 시 이펙트를 적용할 GameplayCue(Actor)
 */
UCLASS()
class RPGPORTFOLIO_API ARPGGC_Player_UsePotionFX : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()
public:
	ARPGGC_Player_UsePotionFX();
protected:
	virtual bool WhileActive_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) override;

	virtual bool OnRemove_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) override;

	// 재생할 사운드
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USoundBase> SoundToPlay;

	// 물약 사용시 이펙트
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystem* UseItemActiveEffect;

	// 물약 사용시 이펙트를 스폰할 SocketName
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName SocketName;
};
