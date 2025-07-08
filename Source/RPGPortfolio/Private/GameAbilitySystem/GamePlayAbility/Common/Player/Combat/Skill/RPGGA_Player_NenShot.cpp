// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/Skill/RPGGA_Player_NenShot.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "WorldStatic/Projectile/ProjectileBase.h"
#include "Component/CombatComponentBase.h"
#include "Component/Player/ObjectPoolComponent.h"
#include "WorldStatic/Weapon/WeaponBase.h"

URPGGA_Player_NenShot::URPGGA_Player_NenShot()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Player_NenShot::ActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (AttackMontage)
	{

		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
	this,TEXT("Player_Attack_Range"), AttackMontage, 1.0f,  NAME_None,
	true, 1.0f, false);

		PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Player_NenShot::OnEndAbilityCallback);
		PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Player_NenShot::OnEndAbilityCallback);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Player_NenShot::OnEndAbilityCallback);
		PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Player_NenShot::OnEndAbilityCallback);
		PlayMontageTask->ReadyForActivation();

		UAbilityTask_WaitGameplayEvent* WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, RPGGameplayTag::Character_Event_SpawnProjectile, nullptr, false, true
		);

		WaitGameplayEvent->EventReceived.AddDynamic(this, &ThisClass::SpawnProjectile);
		WaitGameplayEvent->ReadyForActivation();
	}
}

void URPGGA_Player_NenShot::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URPGGA_Player_NenShot::SpawnProjectile(FGameplayEventData PayloadData)
{ 
	FVector CharacterLocation = GetPlayerCharacterFromActorInfo()->GetActorLocation();
	FVector CharacterForward = GetPlayerCharacterFromActorInfo()->GetActorForwardVector();
	FVector CharacterRight = GetPlayerCharacterFromActorInfo()->GetActorRightVector();
	FVector CharacterUp = GetPlayerCharacterFromActorInfo()->GetActorUpVector();
	FVector ProjectileLocation = CharacterLocation + 
									(CharacterForward * 50.0f) +
										(CharacterUp * 50.0f);
	
	FRotator ProjectileDirection = UKismetMathLibrary::MakeRotFromX(GetPlayerCharacterFromActorInfo()->GetActorForwardVector());
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetPlayerCharacterFromActorInfo();
	SpawnParams.Instigator = GetPlayerCharacterFromActorInfo();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	APooledActor* PooledActor = GetPlayerCharacterFromActorInfo()->GetObjectPoolComponent()->SpawnFromPool(ProjectileLocation, ProjectileDirection);
	if (AProjectileBase* Projectile = Cast<AProjectileBase>(PooledActor))
	{
		Projectile->LaunchProjectile(GetPlayerCharacterFromActorInfo()->GetActorForwardVector(), 700.0f);
		float WeaponDamage = GetPlayerCharacterFromActorInfo()->GetCombatComponent()->GetCharacterCurrentEquippedWeapon()->WeaponDefaultData.WeaponBaseDamage;
		Projectile->DamageEffectSpecHandle = MakePlayerSkillDamageEffectSpecHandle(DamageEffectClass, WeaponDamage, DamageScale);
	}
	
	// FVector CharacterLocation = GetPlayerCharacterFromActorInfo()->GetActorLocation();
	// FVector CharacterUp = GetPlayerCharacterFromActorInfo()->GetActorUpVector();
	//
	// float WeaponDamage = GetPlayerCharacterFromActorInfo()->GetCombatComponent()->GetCharacterCurrentEquippedWeapon()->WeaponDefaultData.WeaponBaseDamage;
	//
	// for (int32 i = 0; i < ProjectileCount; i++)
	// {
	// 	float Angle = (360.0f / ProjectileCount) * i;
	// 	float AngleRadians = FMath::DegreesToRadians(Angle);
	// 	
	// 	FVector NearBodyPosition = FVector(
	// 		FMath::Cos(AngleRadians) * StartRadius,
	// 		FMath::Sin(AngleRadians) * StartRadius,
	// 		0.0f
	// 	);
	// 	
	// 	FVector ProjectileLocation = CharacterLocation + NearBodyPosition + (CharacterUp * ProjectileHeight);
	// 	
	// 	FVector FireDirection = NearBodyPosition.GetSafeNormal();
	// 	FRotator ProjectileDirection = UKismetMathLibrary::MakeRotFromX(FireDirection);
	//
	// 	// ObjectPool 사용
	// 	APooledActor* PooledActor = GetPlayerCharacterFromActorInfo()->GetObjectPoolComponent()->SpawnFromPool(ProjectileLocation, ProjectileDirection);
	// 	if (AProjectileBase* Projectile = Cast<AProjectileBase>(PooledActor))
	// 	{
	// 		// 바깥쪽 방향으로 발사
	// 		Projectile->LaunchProjectile(FireDirection, 700.0f);
	// 		Projectile->DamageEffectSpecHandle = MakePlayerSkillDamageEffectSpecHandle(DamageEffectClass, WeaponDamage, DamageScale);
	// 	}
	// }
}
