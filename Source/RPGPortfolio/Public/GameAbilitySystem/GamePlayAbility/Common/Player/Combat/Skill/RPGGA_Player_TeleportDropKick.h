// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/Skill/PlayerAttackSkillGameplayAbility.h"
#include "RPGGA_Player_TeleportDropKick.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_TeleportDropKick : public UPlayerAttackSkillGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_TeleportDropKick();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName WarpTargetNameDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName WarpTargetNameDistance;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void ForwardDirection();
	
	UFUNCTION()
	void RotateDirection();
	
	UFUNCTION()
	void OnEndAbilityCallback();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* AttackMontage;
	
	UFUNCTION()
	void ApplyEffectsSpecHandleToTargetCallback(FGameplayEventData PayloadData);


	
};
