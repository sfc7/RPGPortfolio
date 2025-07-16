// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/Skill/PlayerAttackSkillGameplayAbility.h"
#include "RPGGA_Player_Nova.generated.h"

class AProjectileBase;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_Nova : public UPlayerAttackSkillGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_Nova();

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
	int32 ProjectileCount = 20;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float StartRadius = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ProjectileHeight = 30.0f; 
};
