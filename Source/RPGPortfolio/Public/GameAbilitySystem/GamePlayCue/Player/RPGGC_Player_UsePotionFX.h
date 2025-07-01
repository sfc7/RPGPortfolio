// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "RPGGC_Player_UsePotionFX.generated.h"

class UNiagaraSystem;
class UParticleSystem;
/**
 * 
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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USoundBase> SoundToPlay;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystem* UseItemActiveEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName SocketName;
};
