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
	virtual void OnHitTargetActor(AActor* _HitActor, float _WeaponBaseDamage, EWeaponAttackType AttackType);
	virtual void OnWeaponPulledFromTargetActor(AActor* _InteractedActor, float _WeaponBaseDamage, EWeaponAttackType AttackType);
};
