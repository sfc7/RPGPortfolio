// LJS


#include "Component/CombatComponentBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "WorldStatic/Weapon/WeaponBase.h"
#include "Components/BoxComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"


void UCombatComponentBase::RegisterSpawnedWeapon(FGameplayTag _WeaponTagToRegister, AWeaponBase* _WeaponToRegister, bool bEquippedWeaponState)
{
	if (!IsValid(_WeaponToRegister))
	{
		UE_LOG(LogTemp, Warning, TEXT("RegisterSpawnedWeapon: Invalid weapon"));
		return;
	}

	if (!_WeaponTagToRegister.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("RegisterSpawnedWeapon: Invalid weapon tag"));
		return;
	}

	if (CharacterWeapons.Contains(_WeaponTagToRegister))
	{
		UE_LOG(LogTemp, Warning, TEXT("RegisterSpawnedWeapon: Weapon already registered"));
		return;
	}

	CharacterWeapons.Emplace(_WeaponTagToRegister, _WeaponToRegister);

	_WeaponToRegister->OnWeaponHitBegin.BindUObject(this, &ThisClass::OnHitTargetActor);
	_WeaponToRegister->OnWeaponHitEnd.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);

	if (bEquippedWeaponState)
	{
		CurrentEquippedWeaponTag = _WeaponTagToRegister;
	}
}

AWeaponBase* UCombatComponentBase::GetCharacterCarriedWeaponByTag(FGameplayTag _WeaponTagToGet) const
{
	if (!_WeaponTagToGet.IsValid())
	{
		return nullptr;
	}
	
	if (!CharacterWeapons.Contains(_WeaponTagToGet))
	{
		return nullptr;
	}

	AWeaponBase* const* const FoundWeaponPtr = CharacterWeapons.Find(_WeaponTagToGet);
	if (!FoundWeaponPtr)
	{
		return nullptr;
	}

	return *FoundWeaponPtr;
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
	if (!IsValid(_Weapon))
	{
		return;
	}

	UBoxComponent* const WeaponCollisionBox = _Weapon->GetWeaponCollisionBox();
	if (!IsValid(WeaponCollisionBox))
	{
		return;
	}
	
	if (_bShouldEnable)
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
	// 토글 타입이 무기라면 SetWeaponCollision으로, 몸부위라면 ToggleBodyCollisionBoxCollsion으로
	if (_ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		AWeaponBase* CurrentWeapon = GetCharacterCurrentEquippedWeapon();

		SetWeaponCollision(CurrentWeapon, _bShouldEnable);
	}
	else
	{
		ToggleBodyCollisionBoxCollsion(_bShouldEnable, _ToggleDamageType); 
	}
}

void UCombatComponentBase::ToggleCarriedWeaponCollision(AWeaponBase* _CarriedWeapon, bool _bShouldEnable, EToggleDamageType _ToggleDamageType)
{
	if (_ToggleDamageType == EToggleDamageType::CarriedWeapon)
	{
		SetWeaponCollision(_CarriedWeapon, _bShouldEnable);
	}

	//TODO : Handlebody collision Boxes
}

void UCombatComponentBase::OnHitTargetActor(AActor* _HitActor, float WeaponBaseDamage, float WeaponAttackRate, EWeaponAttackType AttackType, EWeaponType WeaponType, FName SocketName)
{
	
}

void UCombatComponentBase::OnWeaponPulledFromTargetActor(AActor* _HitActor, float WeaponBaseDamage, float WeaponAttackRate, EWeaponAttackType AttackType, EWeaponType WeaponType)
{
}

void UCombatComponentBase::ToggleBodyCollisionBoxCollsion(bool _bShouldEnable, EToggleDamageType _ToggleDamageType)
{
}

URPGAbilitySystemComponent* UCombatComponentBase::GetRPGAbilitySystemComponent() const
{
	const APlayerCharacterBase* const PlayerCharacter = Cast<APlayerCharacterBase>(GetOwner());
	
	if (!IsValid(PlayerCharacter))
	{
		return nullptr;
	}

	return PlayerCharacter->GetRPGAbilitySystemComponent();
}
