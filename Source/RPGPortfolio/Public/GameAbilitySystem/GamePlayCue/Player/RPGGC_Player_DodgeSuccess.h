// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "RPGGC_Player_DodgeSuccess.generated.h"

class UNiagaraSystem;
/**
 * URPGGC_Player_DodgeSuccess
 * 
 * 회피 성공 시 이펙트를 적용할 GameplayCue(Static)
 */
UCLASS()
class RPGPORTFOLIO_API URPGGC_Player_DodgeSuccess : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
public:
	URPGGC_Player_DodgeSuccess();

protected:
	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;

	// 재생할 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> SoundToPlay;

	// 회피 성공 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* DodgeEffect;

	// 회피 성공 이펙트를 스폰할 SocketName
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;

};
