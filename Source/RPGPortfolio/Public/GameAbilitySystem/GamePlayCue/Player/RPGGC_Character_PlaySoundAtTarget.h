// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "RPGGC_Character_PlaySoundAtTarget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGC_Character_PlaySoundAtTarget_AttackHit : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
public:
	URPGGC_Character_PlaySoundAtTarget_AttackHit();

protected:
	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> SoundToPlay;
};
