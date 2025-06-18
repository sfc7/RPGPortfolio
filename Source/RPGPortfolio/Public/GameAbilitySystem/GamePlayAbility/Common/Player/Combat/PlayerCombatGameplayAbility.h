// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerGameplayAbility.h"
#include "PlayerCombatGameplayAbility.generated.h"

class URPGAT_Player_RotateTarget;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UPlayerCombatGameplayAbility : public UPlayerGameplayAbility
{
	GENERATED_BODY()
public:
	UPlayerCombatGameplayAbility();

	UFUNCTION(BlueprintCallable)
	void RotateTargetTickBeforeAttack(float DeltaTime);
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	
	//Rotate
	UFUNCTION()
	bool FindNearestEnemyBeforeAttack(float BoxExtent);

	UPROPERTY()
	AActor* RotateTargetActor;

	UPROPERTY()
	FRotator FindRototation;
	//Rotate
};
