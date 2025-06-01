// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Monster/MonsterGameplayAbility.h"
#include "RPGGA_Monster_Attack_Melee.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Monster_Attack_Melee : public UMonsterGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Monster_Attack_Melee();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnEventReceived(FGameplayEventData PayloadData);
	
	UFUNCTION()
	void OnEndAbilityCallback();

	UFUNCTION()
	void ApplyDamage(FGameplayEventData PayloadData);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag HitSoundGameplayCueTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FScalableFloat DamageScale;

	AActor* LocalTargetActor;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UAnimMontage*> AttackMontages;
};
