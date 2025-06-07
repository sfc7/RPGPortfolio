// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/PlayerCombatGameplayAbility.h"
#include "RPGGA_Player_HitReact.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_HitReact : public UPlayerCombatGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_HitReact();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* HitReactMontage_Front;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* HitReactMontage_Back;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* HitReactMontage_Left;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* HitReactMontage_Right;
	
	UFUNCTION()
	void OnEndAbilityCallback();
};
