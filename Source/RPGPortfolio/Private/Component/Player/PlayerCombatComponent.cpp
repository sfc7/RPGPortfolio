// LJS


#include "Component/Player/PlayerCombatComponent.h"
#include "WorldStatic/Weapon/PlayerWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"

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

void UPlayerCombatComponent::OnHitTargetActor(AActor* _HitActor, float _WeaponBaseDamage)
{
	if (OVerlappedActors.Contains(_HitActor)) return;

	OVerlappedActors.AddUnique(_HitActor);

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = _HitActor;
	EventData.EventMagnitude = _WeaponBaseDamage;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		RPGGameplayTag::Character_Event_AttackHit_Melee,
		EventData
	);
}

void UPlayerCombatComponent::OnWeaponPulledFromTargetActor(AActor* _InteractedActor, float _WeaponBaseDamage)
{
}
