// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Monster/MonsterGameplayAbility.h"
#include "RPGGA_Monster_Attack_Meteor.generated.h"

class AAOEBase;
class AProjectileBase;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Monster_Attack_Meteor : public UMonsterGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Monster_Attack_Meteor();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION()
	void OnEndAbilityCallback();

	UFUNCTION()
	void SpawnMeteor(FGameplayEventData PayloadData);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FScalableFloat DamageScale;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MeteorTraceRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* PortalEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MeteorRandomOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AAOEBase> MeteorDecalClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AProjectileBase> ProjectileClass;

protected:
	UFUNCTION()
	void ShowWeapon(FGameplayEventData PayloadData);

	UFUNCTION()
	void HideWeapon(FGameplayEventData PayloadData);
};
