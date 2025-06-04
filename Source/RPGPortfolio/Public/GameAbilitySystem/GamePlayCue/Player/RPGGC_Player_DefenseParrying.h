// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "RPGGC_Player_DefenseParrying.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGC_Player_DefenseParrying : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
public:
	URPGGC_Player_DefenseParrying();

protected:
	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> SoundToPlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* ParryingEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;
};
