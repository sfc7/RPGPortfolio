// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "RPGGC_Player_DodgeSuccess.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGC_Player_DodgeSuccess : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
public:
	URPGGC_Player_DodgeSuccess();

protected:
	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> SoundToPlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* DodgeEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;

};
