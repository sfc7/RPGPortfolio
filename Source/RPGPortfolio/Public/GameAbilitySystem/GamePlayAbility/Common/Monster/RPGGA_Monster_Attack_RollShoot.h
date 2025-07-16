// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Monster/MonsterGameplayAbility.h"
#include "RPGGA_Monster_Attack_RollShoot.generated.h"

class AProjectileBase;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Monster_Attack_RollShoot : public UMonsterGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Monster_Attack_RollShoot();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION()
	void OnEndAbilityCallback();

	UFUNCTION()
	void SpawnBullet(FGameplayEventData PayloadData);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FScalableFloat DamageScale;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float BulletTraceRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName BulletMuzzle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AProjectileBase> ProjectileClass;

	UFUNCTION()
	void ShowWeapon(FGameplayEventData PayloadData);

	UFUNCTION()
	void HideWeapon(FGameplayEventData PayloadData);
};
