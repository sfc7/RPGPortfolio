// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "RPGGC_Player_DefenseSuccess.generated.h"

class UNiagaraSystem;
/**
 * ARPGGC_Player_Defense
 * 
 * 방어 성공 시 이펙트를 적용할 GameplayCue(Static)
 */
UCLASS()
class RPGPORTFOLIO_API URPGGC_Player_DefenseSuccess : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
public:
	URPGGC_Player_DefenseSuccess();

protected:
	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;

	// 재생할 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> SoundToPlay;

	// 디펜스 성공 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* DefenseEffect;

	// 디펜스 성공 스폰할 SocketName
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;

};
