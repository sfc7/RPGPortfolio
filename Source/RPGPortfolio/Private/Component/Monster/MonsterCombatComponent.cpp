// LJS


#include "Component/Monster/MonsterCombatComponent.h"

void UMonsterCombatComponent::OnHitTargetActor(AActor* _HitActor, float _WeaponBaseDamage, EWeaponAttackType AttackType)
{
	if(IsValid(_HitActor))	
	{
		
	}
}

void UMonsterCombatComponent::OnWeaponPulledFromTargetActor(AActor* _InteractedActor, float _WeaponBaseDamage, EWeaponAttackType AttackType)
{
	
}
