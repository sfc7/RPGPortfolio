// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerGameplayAbility.h"
#include "RPGGA_Player_AttackBuffSkill.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_AttackBuffSkill : public UPlayerGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_AttackBuffSkill();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnEndAbilityCallback();

	UFUNCTION()
	void ApplyFXGameplayCue(FGameplayEventData PayloadData);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories=GameplayCue))
	FGameplayTag BuffFXGameplayCue;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* BuffCastingMontage;
};
