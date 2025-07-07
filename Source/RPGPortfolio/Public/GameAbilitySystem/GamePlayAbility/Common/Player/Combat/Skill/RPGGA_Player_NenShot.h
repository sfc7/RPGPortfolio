// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/Skill/PlayerAttackSkillGameplayAbility.h"
#include "RPGGA_Player_NenShot.generated.h"

class AProjectileBase;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_NenShot : public UPlayerAttackSkillGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_NenShot();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* AttackMontage;
	
	UFUNCTION()
	void OnEndAbilityCallback();

	UFUNCTION()
	void SpawnProjectile(FGameplayEventData PayloadData);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AProjectileBase> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FScalableFloat DamageScale;
};
