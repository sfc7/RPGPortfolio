// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerGameplayAbility.h"
#include "RPGGA_Player_Defense.generated.h"

class UPlayerGameplayAbility;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_Defense : public UPlayerGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_Defense();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


	UFUNCTION()
	void SuccessDefenseCallback(FGameplayEventData PayloadData);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* DefenseMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories=GameplayCue))
	FGameplayTag DefenseEffectGamePlayCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories=GameplayCue))
	FGameplayTag DefenseSuccessGamePlayCue;

	float DefenseActivateTime;
	
	UFUNCTION()
	void OnEndAbilityCallback();
	
	// Parry 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories=GameplayCue))
	FGameplayTag DefenseParryingGamePlayCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UPlayerGameplayAbility* ParryingAttackGA;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag ParryingTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> InvincibleEffectClass;

	FTimerHandle ParryingDelayTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* ParryingInputMappingContext;
	
	UFUNCTION()
	void ResetParryingAttackTimer();

	UFUNCTION()
	void SetParryingAttackReady();
	// Parry 
	

	
};
