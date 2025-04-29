// LJS


#include "Component/CombatComponentBase.h"
#include "WorldStatic/Weapon/WeaponBase.h"


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
