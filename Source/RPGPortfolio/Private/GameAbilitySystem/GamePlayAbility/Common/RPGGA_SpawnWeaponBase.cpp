// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/RPGGA_SpawnWeaponBase.h"
#include "Character/RPGCharacterBase.h"
#include "Component/CombatComponentBase.h"
#include "WorldStatic/Weapon/WeaponBase.h"

URPGGA_SpawnWeaponBase::URPGGA_SpawnWeaponBase()
{
	AbilityActivationPolicy = ERPGGameplayAbilityActivationType::OnGiven;
}

void URPGGA_SpawnWeaponBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	OwnerCharacter = CastChecked<ARPGCharacterBase>(GetAvatarActorFromActorInfo());
		
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParameters.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
	SpawnParameters.Owner = OwnerCharacter;
	SpawnParameters.Instigator = CastChecked<APawn>(OwnerCharacter);
	
	FVector SocketLocation = OwnerCharacter->GetMesh()->GetSocketLocation(SocketNameToAttach);
	FRotator SocketRotation = OwnerCharacter->GetMesh()->GetSocketRotation(SocketNameToAttach);

	AWeaponBase* PlayerWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, SocketLocation, SocketRotation, SpawnParameters);
	if (IsValid(PlayerWeapon))
	{ 
		PlayerWeapon->AttachToComponent(GetOwningComponentFromActorInfo(), // GetOwningComponentFromActorInfo() == GetSkeletalMeshComponentFromActorInfo;
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true),
			SocketNameToAttach);
	}

	GetCombatComponentFromActorInfo()->RegisterSpawnedWeapon(WeaponTagtoRegister, PlayerWeapon, bShouldRegisterEquipWeapon);
}

void URPGGA_SpawnWeaponBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
