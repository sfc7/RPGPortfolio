// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "RPGGA_Player_Attack.generated.h"

class UAnimMontage;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_Attack : public URPGGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_Attack();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	
	TMap<int32, UAnimMontage*> LightAttackMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	uint8 CurrentComboCount;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


protected:
	UFUNCTION()
	void OnEndAbilityCallback();
	
	UFUNCTION()
	void ResetComboCount();

	UFUNCTION()
	void ResetComboCountTimer();

	FTimerHandle ComboTimerHandle;
};
