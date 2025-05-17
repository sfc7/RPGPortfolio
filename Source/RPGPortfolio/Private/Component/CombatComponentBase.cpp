// LJS


#include "Component/CombatComponentBase.h"
#include "WorldStatic/Weapon/WeaponBase.h"
#include "Components/BoxComponent.h"

void UCombatComponentBase::RegisterSpawnedWeapon(FGameplayTag _WeaponTagToRegister, AWeaponBase* _WeaponToRegister, bool bEquippedWeaponState)
{
	if (_WeaponToRegister && !CharacterWeapons.Contains(_WeaponTagToRegister)) CharacterWeapons.Emplace(_WeaponTagToRegister, _WeaponToRegister);
	
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

void UCombatComponentBase::ToggleWeaponCollision(bool _bShouldEnable, EToggleDamageType _ToggleDamageType)
{
	if (_ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		AWeaponBase* CurrentWeapon = GetCharacterCurrentEquippedWeapon();

		if (CurrentWeapon && _bShouldEnable)
		{
			CurrentWeapon->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			UE_LOG(LogTemp, Warning, TEXT("QueryOnly"));
		}
		else
		{
			CurrentWeapon->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			UE_LOG(LogTemp, Warning, TEXT("NoCollision"));
		}
	}
}
