// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerGameplayAbility.h"
#include "RPGGA_Player_HeavyAttack.generated.h"

class UAnimMontage;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_HeavyAttack : public UPlayerGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_HeavyAttack();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<int32, UAnimMontage*> HeavyAttackMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnEndAbilityCallback();
	
	UFUNCTION()
	void ApplyEffectsSpecHandleToTargetCallback(FGameplayEventData PayloadData);
		
	float CurrentLightAttackComboCount = 1;

	AActor* LocalTargetActor;
};
