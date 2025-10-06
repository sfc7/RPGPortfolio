// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "RPGGC_Player_DefenseParrying.generated.h"

class UNiagaraSystem;
/**
 * URPGGC_Player_DefenseParrying
 * 
 * 패링 성공 시 이펙트를 적용할 GameplayCue(Static)
 */
UCLASS()
class RPGPORTFOLIO_API URPGGC_Player_DefenseParrying : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
public:
	URPGGC_Player_DefenseParrying();

protected:
	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;

	//재생할 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> SoundToPlay;

	//패링 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* ParryingEffect;

	//패링 스폰할 SocketName
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;
};
