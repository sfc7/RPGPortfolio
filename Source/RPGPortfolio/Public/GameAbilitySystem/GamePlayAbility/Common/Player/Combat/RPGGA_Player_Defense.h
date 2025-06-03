// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerGameplayAbility.h"
#include "RPGGA_Player_Defense.generated.h"

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

	UFUNCTION()
	void SuccessDefenseCallback(FGameplayEventData PayloadData);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* DefenseMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories=GameplayCue))
	FGameplayTag DefenseEffectGamePlayCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories=GameplayCue))
	FGameplayTag DefenseSuccessGamePlayCue;
	
	UFUNCTION()
	void OnEndAbilityCallback();
	
};
