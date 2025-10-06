// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "RPGGC_Character_PlaySoundAtTarget.generated.h"

/**
 * URPGGC_Character_PlaySoundAtTarget_AttackHit
 * 
 * 공격 받을 때 사운드 재생할 GameplayCue (Static)
 */
UCLASS()
class RPGPORTFOLIO_API URPGGC_Character_PlaySoundAtTarget_AttackHit : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
public:
	URPGGC_Character_PlaySoundAtTarget_AttackHit();

protected:
	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;

	// 재생할 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> SoundToPlay;
};
