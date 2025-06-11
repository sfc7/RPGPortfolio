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
	virtual void OnHitTargetActor(AActor* _HitActor);
	virtual void OnWeaponPulledFromTargetActor(AActor* _InteractedActor, float _WeaponBaseDamage, EWeaponAttackType AttackType) override;

protected:
	virtual void ToggleBodyCollisionBoxCollsion(bool _bShouldEnable, EToggleDamageType _ToggleDamageType) override;
};
