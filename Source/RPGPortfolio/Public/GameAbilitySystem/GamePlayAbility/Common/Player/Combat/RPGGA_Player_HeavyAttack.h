// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/PlayerCombatGameplayAbility.h"
#include "RPGGA_Player_HeavyAttack.generated.h"

class UAnimMontage;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_HeavyAttack : public UPlayerCombatGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_HeavyAttack();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	
	UFUNCTION()
	void OnEndAbilityCallback();
	
	UFUNCTION()
	void ApplyEffectsSpecHandleToTargetCallback(FGameplayEventData PayloadData);
		
	float CurrentLightAttackComboCount = 1;

	AActor* LocalTargetActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<int32, UAnimMontage*> HeavyAttackMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
};	
