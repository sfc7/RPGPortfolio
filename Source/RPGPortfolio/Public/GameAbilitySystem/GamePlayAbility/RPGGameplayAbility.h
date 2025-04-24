// LJS

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RPGGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class ERPGGameplayAbilityActivationType : uint8
{
	OnTriggered,
	OnGiven
};
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
 
	UPROPERTY(EditDefaultsOnly)
	ERPGGameplayAbilityActivationType AbilityActivationPolicy = ERPGGameplayAbilityActivationType::OnTriggered;
};
