// LJS

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RPGGameplayAbility.generated.h"

class UCombatComponentBase;
class URPGAbilitySystemComponent;

//@ GameplayAbility 활성화 타입 열거형
UENUM(BlueprintType)
enum class ERPGGameplayAbilityActivationType : uint8
{
	OnTriggered,
	OnGiven
};

/**
 * URPGGameplayAbility
 * 
 * GAS 시스템의 제일 기본 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API URPGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	//@ 어빌리티 활성화 정책 (트리거 Or 부여)
	UPROPERTY(EditDefaultsOnly, Category = "GA")
	ERPGGameplayAbilityActivationType AbilityActivationPolicy = ERPGGameplayAbilityActivationType::OnTriggered;

	UFUNCTION(BlueprintPure, Category = "GA")
	UCombatComponentBase* GetCombatComponentFromActorInfo() const;

	//@ 액터 정보에서 컴뱃 컴포넌트 가져오기 
	UFUNCTION(BlueprintPure, Category = "GA")
	URPGAbilitySystemComponent* GetRPGAbilitySystemComponentFromActorInfo() const;

	//@ 타겟 액터에게 GameplayEffectSpec 적용
	FActiveGameplayEffectHandle ApplyEffectsSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& _SpecHandle);

	//@ 액터에게 GameplayTag 추가
	UFUNCTION(BlueprintCallable, Category = "GA")
	void AddGameplayTag(AActor* _Actor,FGameplayTag _GameplayTag);

	//@ 액터에게 GameplayTag 제거
	UFUNCTION(BlueprintCallable, Category = "GA")
	void RemoveGameplayTag(AActor* _Actor,FGameplayTag _GameplayTag);

	//@ 액터가 GameplayTag 가지고 있는지 체크
	UFUNCTION(BlueprintCallable, Category = "GA")
	bool HasMatchingGameplayTag(FGameplayTag _GameplayTag);

	//@ 충돌 액터들에게 GameplayEffectSpec 적용 
	UFUNCTION(BlueprintCallable, Category = "GA")
	void ApplyGameplayEffectgSpecHandleToHitResults(const FGameplayEffectSpecHandle& SpecHandle, const TArray<FHitResult>& HitResults);
};
