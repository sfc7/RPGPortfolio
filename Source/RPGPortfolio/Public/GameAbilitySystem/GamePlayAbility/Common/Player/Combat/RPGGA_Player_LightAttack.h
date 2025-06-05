// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/PlayerCombatGameplayAbility.h"
#include "RPGGA_Player_LightAttack.generated.h"

class UAnimMontage;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_LightAttack : public UPlayerCombatGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_LightAttack();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION()
	void OnEndAbilityCallback();

	UFUNCTION()
	void ApplyEffectsSpecHandleToTargetCallback(FGameplayEventData PayloadData);
	
	UFUNCTION()
	void ResetComboCount();

	UFUNCTION()
	void ResetComboCountTimer();

	FTimerHandle ComboTimerHandle;

	float UsedComboCount;

	AActor* LocalTargetActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<int32, UAnimMontage*> LightAttackMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> DamageEffectClass;
private:
	
};
