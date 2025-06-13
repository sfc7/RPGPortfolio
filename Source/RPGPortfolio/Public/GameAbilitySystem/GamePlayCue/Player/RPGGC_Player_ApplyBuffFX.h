// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "RPGGC_Player_ApplyBuffFX.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UParticleSystem;
class UParticleSystemComponent;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API ARPGGC_Player_ApplyBuffFX : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()
public:
	ARPGGC_Player_ApplyBuffFX();
protected:
	virtual bool WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

	virtual bool OnRemove_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USoundBase> SoundToPlay;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystem* BuffActiveEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystem* BuffEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName LeftHandSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName RightHandSocketName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* LeftHandActiveEffect;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* RightHandActiveBuffEffect;
	
};
