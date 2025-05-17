// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerGameplayAbility.h"
#include "RPGGA_Player_LightAttack.generated.h"

class UAnimMontage;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_LightAttack : public UPlayerGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_LightAttack();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<int32, UAnimMontage*> LightAttackMontages;
	
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
