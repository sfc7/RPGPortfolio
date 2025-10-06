// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Monster/MonsterGameplayAbility.h"
#include "NiagaraSystem.h" 
#include "RPGGA_Montser_Death_Base.generated.h"

class UNiagaraSystem;

/**
 * URPGGA_Montser_Death_Base
 * 
 * 몬스터 죽음을 처리하는 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Montser_Death_Base : public UMonsterGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Montser_Death_Base();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	//@ 죽음 Montaage
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UAnimMontage*> DeathMontages;

	//@ 죽음 시 재생되는 사운드의 GameplayTag
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag DeathSoudnGameplayTag;

	//@ 죽음 시 생성되는 이펙트
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<UNiagaraSystem> DeathNiagaraEffect;

	//@ 어빌리티 종료 콜백 함수
	UFUNCTION()
	void OnEndAbilityCallback();
};
