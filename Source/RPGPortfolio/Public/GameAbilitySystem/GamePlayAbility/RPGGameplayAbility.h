// LJS

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RPGGameplayAbility.generated.h"

class UCombatComponentBase;
class URPGAbilitySystemComponent;

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
 
	UPROPERTY(EditDefaultsOnly, Category = "GA")
	ERPGGameplayAbilityActivationType AbilityActivationPolicy = ERPGGameplayAbilityActivationType::OnTriggered;

	UFUNCTION(BlueprintPure, Category = "GA")
	UCombatComponentBase* GetCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "GA")
	URPGAbilitySystemComponent* GetRPGAbilitySystemComponentFromActorInfo() const;

	FActiveGameplayEffectHandle ApplyEffectsSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& _SpecHandle);

	//bp_NativeApplyEffectsSpecHandleToTarget;

	UFUNCTION(BlueprintCallable, Category = "GA")
	void AddGameplayTag(AActor* _Actor,FGameplayTag _GameplayTag);

	UFUNCTION(BlueprintCallable, Category = "GA")
	void RemoveGameplayTag(AActor* _Actor,FGameplayTag _GameplayTag);

	UFUNCTION(BlueprintCallable, Category = "GA")
	bool HasMatchingGameplayTag(FGameplayTag _GameplayTag);
};
