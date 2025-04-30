// LJS


#include "GameAbilitySystem/GamePlayAbility/Player/State/Player_EquipWeapon.h"
#include "Character/PlayerCharacter.h"
#include "Component/CombatComponentBase.h"
#include "WorldStatic/Weapon/PlayerWeapon.h"

UPlayer_EquipWeapon::UPlayer_EquipWeapon()
{
	AbilityActivationPolicy = ERPGGameplayAbilityActivationType::OnGiven;
}

void UPlayer_EquipWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	OwnerCharacter = CastChecked<APlayerCharacter>(GetAvatarActorFromActorInfo());
		
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParameters.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
	SpawnParameters.Owner = OwnerCharacter;
	SpawnParameters.Instigator = CastChecked<APawn>(OwnerCharacter);

	FVector SocketLocation = OwnerCharacter->GetMesh()->GetSocketLocation(SocketNameToAttach);
	FRotator SocketRotation = OwnerCharacter->GetMesh()->GetSocketRotation(SocketNameToAttach);

	APlayerWeapon* PlayerWeapon = GetWorld()->SpawnActor<APlayerWeapon>(WeaponClass, SocketLocation, SocketRotation, SpawnParameters);
	if (IsValid(PlayerWeapon))
	{ 
		PlayerWeapon->AttachToComponent(GetOwningComponentFromActorInfo(), // GetOwningComponentFromActorInfo() == GetSkeletalMeshComponentFromActorInfo;
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true),
			SocketNameToAttach);
	}

	GetCombatComponentFromActorInfo()->RegisterSpawnedWeapon(WeaponTagtoRegister, PlayerWeapon);
}

void UPlayer_EquipWeapon::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

