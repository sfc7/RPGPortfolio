// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "RPGGC_Player_Sound_AttackHit.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGC_Player_Sound_AttackHit : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
public:
	URPGGC_Player_Sound_AttackHit();

	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> SoundToPlay;
};
