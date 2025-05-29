// LJS


#include "Component/Monster/MonsterCombatComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "AbilitySystemBlueprintLibrary.h"

void UMonsterCombatComponent::OnHitTargetActor(AActor* _HitActor, float _WeaponBaseDamage, EWeaponAttackType AttackType)
{
	if (OverlappedActors.Contains(_HitActor))
	{
		return;
	}

	OverlappedActors.AddUnique(_HitActor);

	//TODO:: Implement block check
	bool bIsValidBlock = false;

	const bool bIsPlayerBlocking = false;
	const bool bIsMyAttackUnblokcable = false;

	if (bIsPlayerBlocking && !bIsMyAttackUnblokcable)
	{
		//TODO::check if the block is valid
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = _HitActor;
	
	if (bIsValidBlock)
	{
		//TODO::Handle successful block
	}
	else
	{		
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			RPGGameplayTag::Character_Event_AttackHit_Melee,
			EventData
		);
	}
}

void UMonsterCombatComponent::OnWeaponPulledFromTargetActor(AActor* _InteractedActor, float _WeaponBaseDamage, EWeaponAttackType AttackType)
{
	
}
