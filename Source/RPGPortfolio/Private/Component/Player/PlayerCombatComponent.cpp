// LJS


#include "Component/Player/PlayerCombatComponent.h"
#include "WorldStatic/Weapon/PlayerWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "RPGFunc.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"

void UPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	InitRegisterGameplayTagEvent();
}

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

	if (AttackType == EWeaponAttackType::Heavy || AttackType == EWeaponAttackType::Parrying)
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

void UPlayerCombatComponent::OnParryingStateChange(const FGameplayTag CallbackTag, int32 NewCount)
{	
	APlayerController* PC = GetOwningController<APlayerController>();
	if (!PC) return;

	ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	APlayerCharacterBase* PlayerCharacter = GetOwningPawn<APlayerCharacterBase>();
	if (!IsValid(Subsystem) || !PlayerCharacter->GetParryingInputMappingContext())
		return;

	if (NewCount > 0)
	{
		Subsystem->AddMappingContext(PlayerCharacter->GetParryingInputMappingContext(), 1);
	}
	else
	{
		Subsystem->RemoveMappingContext(PlayerCharacter->GetParryingInputMappingContext());
	}
}

void UPlayerCombatComponent::InitRegisterGameplayTagEvent()
{
	GetRPGAbilitySystemComponent()->RegisterGameplayTagEvent(RPGGameplayTag::Player_Status_CanParryingAttack,
	EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnParryingStateChange);
}
