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
 * ARPGGC_Player_ApplyBuffFX
 * 
 * 버프 스킬 사용시 양손에 이펙트를 적용할 GameplayCue(Actor)
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

	// 재생할 사운드
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USoundBase> SoundToPlay;

	// 버프 스킬 사용 당시의 이펙트
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystem* BuffActiveEffect;

	// 버프 스킬 사용 시 지속적으로 나타낼 이펙트
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystem* BuffEffect;

	// 왼손 SocketName
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName LeftHandSocketName;

	// 오른손 SocketName
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName RightHandSocketName;

	// 버프 스킬 사용 시 왼손에 지속적으로 나타낼 이펙트  
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* LeftHandActiveEffect;

	// 버프 스킬 사용 시 오른손에 지속적으로 나타낼 이펙트  
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* RightHandActiveBuffEffect;
	
};
