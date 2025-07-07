// LJS

#pragma once

#include "CoreMinimal.h"
#include "Component/CombatComponentBase.h"
#include "MonsterCombatComponent.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UMonsterCombatComponent : public UCombatComponentBase
{
	GENERATED_BODY()
public:
	virtual void RegisterSpawnedWeapon(FGameplayTag _WeaponTagToRegister, AWeaponBase* _WeaponToRegister, bool bEquippedWeaponState = false) override;
	
	virtual void OnHitTargetActor(AActor* _HitActor, float WeaponBaseDamage, float WeaponAttackRate, EWeaponAttackType AttackType, EWeaponType WeaponType, FName SocketName = NAME_None) override;
	virtual void OnWeaponPulledFromTargetActor(AActor* _HitActor, float WeaponBaseDamage, float WeaponAttackRate, EWeaponAttackType AttackType, EWeaponType WeaponType) override;

protected:
	virtual void ToggleBodyCollisionBoxCollsion(bool _bShouldEnable, EToggleDamageType _ToggleDamageType) override;
};
