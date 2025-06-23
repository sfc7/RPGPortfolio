// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "RPGGC_Player_AttackHit.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGC_Player_AttackHit : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
public:
	URPGGC_Player_AttackHit();

protected:
	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* AttackHitEffect;
};
