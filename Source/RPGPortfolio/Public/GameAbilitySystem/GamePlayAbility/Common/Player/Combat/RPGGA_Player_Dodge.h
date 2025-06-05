// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/PlayerCombatGameplayAbility.h"
#include "RPGGA_Player_Dodge.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_Dodge : public UPlayerCombatGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_Dodge();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* DodgeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName WarpTargetNameDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName WarpTargetNameDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FScalableFloat Distance;
	
	UFUNCTION()
	void OnEndAbilityCallback();

	UFUNCTION()
	void CalcDodgeDirectionAndDistance();

	
	FVector DodgeDirection;
	
};
