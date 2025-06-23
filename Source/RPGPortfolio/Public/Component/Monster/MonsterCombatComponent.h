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
	
	virtual void OnHitTargetActor(AActor* _HitActor, float _WeaponBaseDamage, EWeaponAttackType AttackType, FName EquipSocketName = NAME_None) override;
	virtual void OnWeaponPulledFromTargetActor(AActor* _InteractedActor, float _WeaponBaseDamage, EWeaponAttackType AttackType) override;

protected:
	virtual void ToggleBodyCollisionBoxCollsion(bool _bShouldEnable, EToggleDamageType _ToggleDamageType) override;
};
