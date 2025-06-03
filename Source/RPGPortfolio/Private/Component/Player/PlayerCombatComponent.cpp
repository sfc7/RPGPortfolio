// LJS


#include "Component/Player/PlayerCombatComponent.h"
#include "WorldStatic/Weapon/PlayerWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "RPGFunc.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"

APlayerWeapon* UPlayerCombatComponent::GetPlayerCarriedWeaponByTag(FGameplayTag _WeaponTagToGet) const
{
	return Cast<APlayerWeapon>(GetCharacterCarriedWeaponByTag(_WeaponTagToGet));
}

APlayerWeapon* UPlayerCombatComponent::GetPlayerCurrentEquippedWeapon() const
{
	return Cast<APlayerWeapon>(GetCharacterCurrentEquippedWeapon());
}

float UPlayerCombatComponent::GetPlayerCurrentEquippedWeaponDamageAtLevel() const
{
	return GetPlayerCurrentEquippedWeapon()->WeaponDefaultData.WeaponBaseDamage;
}

void UPlayerCombatComponent::OnHitTargetActor(AActor* _HitActor, float _WeaponBaseDamage, EWeaponAttackType AttackType)
{
	if (OverlappedActors.Contains(_HitActor)) return;

	OverlappedActors.AddUnique(_HitActor);

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = _HitActor;
	EventData.EventMagnitude = _WeaponBaseDamage;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		RPGGameplayTag::Character_Event_AttackHit_Melee,
		EventData
	);

	if (AttackType == EWeaponAttackType::Heavy)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			RPGGameplayTag::Player_Event_HitPause,
			FGameplayEventData()
		);
	}

}

void UPlayerCombatComponent::OnWeaponPulledFromTargetActor(AActor* _InteractedActor, float _WeaponBaseDamage, EWeaponAttackType AttackType)
{
	if (AttackType == EWeaponAttackType::Heavy)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			RPGGameplayTag::Player_Event_HitPause,
			FGameplayEventData()
		);
	}

}
