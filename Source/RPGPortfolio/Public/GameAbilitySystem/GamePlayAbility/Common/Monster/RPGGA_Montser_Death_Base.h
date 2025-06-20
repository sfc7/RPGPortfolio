// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Monster/MonsterGameplayAbility.h"
#include "NiagaraSystem.h" 
#include "RPGGA_Montser_Death_Base.generated.h"

class UNiagaraSystem;

UCLASS()
class RPGPORTFOLIO_API URPGGA_Montser_Death_Base : public UMonsterGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Montser_Death_Base();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UAnimMontage*> DeathMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag DeathSoudnGameplayTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<UNiagaraSystem> DeathNiagaraEffect;

	
	UFUNCTION()
	void OnEndAbilityCallback();
};
