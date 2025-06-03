// LJS


#include "Component/Monster/MonsterCombatComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "RPGFunc.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"

void UMonsterCombatComponent::OnHitTargetActor(AActor* _HitActor, float _WeaponBaseDamage, EWeaponAttackType AttackType)
{
	if (OverlappedActors.Contains(_HitActor))
	{
		return;
	}

	OverlappedActors.AddUnique(_HitActor);

	bool bIsFacing = false;

	URPGAbilitySystemComponent* ASC = CastChecked<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(_HitActor));
	const bool bIsPlayerDefensing = ASC->HasMatchingGameplayTag(RPGGameplayTag::Player_Status_Defense);
	bool bIsUnDefendableAttack = false;

	if (bIsPlayerDefensing && !bIsUnDefendableAttack)
	{
		bIsFacing = URPGFunc::IsValidDefense(GetOwningPawn(), _HitActor);
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = _HitActor;
	
	if (bIsFacing)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			_HitActor,
			RPGGameplayTag::Player_Event_DefenseSuccess,
			EventData
		);
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
