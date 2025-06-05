// LJS


#include "Component/CombatComponentBase.h"

#include "Character/Player/PlayerCharacterBase.h"
#include "WorldStatic/Weapon/WeaponBase.h"
#include "Components/BoxComponent.h"


void UCombatComponentBase::RegisterSpawnedWeapon(FGameplayTag _WeaponTagToRegister, AWeaponBase* _WeaponToRegister, bool bEquippedWeaponState)
{
	if (_WeaponToRegister && !CharacterWeapons.Contains(_WeaponTagToRegister)) CharacterWeapons.Emplace(_WeaponTagToRegister, _WeaponToRegister);

	_WeaponToRegister->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
	_WeaponToRegister->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);
	
	if (bEquippedWeaponState) CurrentEquippedWeaponTag = _WeaponTagToRegister;
}

AWeaponBase* UCombatComponentBase::GetCharacterCarriedWeaponByTag(FGameplayTag _WeaponTagToGet) const
{
	if (CharacterWeapons.Contains(_WeaponTagToGet))
	{
		AWeaponBase* const* FoundWeapon = CharacterWeapons.Find(_WeaponTagToGet);
		if (FoundWeapon)
		{
			return *FoundWeapon;
		}
	}

	return nullptr;
}


AWeaponBase* UCombatComponentBase::GetCharacterCurrentEquippedWeapon() const
{
	if (!CurrentEquippedWeaponTag.IsValid()) return nullptr; 
	
	return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}

AWeaponBase* UCombatComponentBase::GetCharacterEquippedWeaponByTag(FGameplayTag _WeaponTagToGet) const
{
	if (!_WeaponTagToGet.IsValid()) return nullptr; 
	
	return GetCharacterCarriedWeaponByTag(_WeaponTagToGet);
}

void UCombatComponentBase::SetWeaponCollision(AWeaponBase* _Weapon, bool _bShouldEnable)
{
	if (_Weapon && _bShouldEnable)
	{
		_Weapon->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		_Weapon->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		OverlappedActors.Empty();
	}
}

void UCombatComponentBase::ToggleCurrentWeaponCollision(bool _bShouldEnable, EToggleDamageType _ToggleDamageType)
{
	if (_ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		AWeaponBase* CurrentWeapon = GetCharacterCurrentEquippedWeapon();

		SetWeaponCollision(CurrentWeapon, _bShouldEnable);
	}

	//TODO : Handlebody collision Boxes
}

void UCombatComponentBase::ToggleCarriedWeaponCollision(AWeaponBase* _CarriedWeapon, bool _bShouldEnable, EToggleDamageType _ToggleDamageType)
{
	if (_ToggleDamageType == EToggleDamageType::CarriedWeapon)
	{
		SetWeaponCollision(_CarriedWeapon, _bShouldEnable);
	}

	//TODO : Handlebody collision Boxes
}

void UCombatComponentBase::OnHitTargetActor(AActor* _HitActor, float _WeaponBaseDamage, EWeaponAttackType AttackType)
{

}

void UCombatComponentBase::OnWeaponPulledFromTargetActor(AActor* _InteractedActor, float _WeaponBaseDamage, EWeaponAttackType AttackType)
{
}
URPGAbilitySystemComponent* UCombatComponentBase::GetRPGAbilitySystemComponent() const
{
	APlayerCharacterBase* PlayerCharacter = Cast<APlayerCharacterBase>(GetOwner());
	return PlayerCharacter->GetRPGAbilitySystemComponent();
}