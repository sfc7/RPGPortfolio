// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/Skill/RPGGA_Player_Nova.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "WorldStatic/Projectile/ProjectileBase.h"
#include "Component/CombatComponentBase.h"
#include "Component/Player/ObjectPoolComponent.h"
#include "WorldStatic/Weapon/WeaponBase.h"
#include "Component/Player/PlayerUIComponent.h"
#include "Component/Player/PlayerSkillComponent.h"

URPGGA_Player_Nova::URPGGA_Player_Nova()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Player_Nova::ActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CommitAbility(Handle, ActorInfo, ActivationInfo);

	if (GetPlayerCharacterFromActorInfo()->GetPlayerSkillComponent())
	{
		int32 QuickSlotIndex = GetPlayerCharacterFromActorInfo()->GetPlayerSkillComponent()->FindQuickSlotIndexByTag(RPGGameplayTag::Player_Ability_Skill_Nova);
        
		GetPlayerCharacterFromActorInfo()->GetPlayerUIComponent()->OnSkillCooldownBeginDelegate.Broadcast(
			QuickSlotIndex, 
			RPGGameplayTag::Player_Ability_Skill_Nova, 
			GetCooldownTimeRemaining(), 
			GetCooldownTimeRemaining()
		);
	}
	
	if (AttackMontage)
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
	this,TEXT("Player_Skill_Nova"), AttackMontage, 1.0f,  NAME_None,
	true, 1.0f, false);

		PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Player_Nova::OnEndAbilityCallback);
		PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Player_Nova::OnEndAbilityCallback);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Player_Nova::OnEndAbilityCallback);
		PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Player_Nova::OnEndAbilityCallback);
		PlayMontageTask->ReadyForActivation();

		UAbilityTask_WaitGameplayEvent* WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, RPGGameplayTag::Character_Event_SpawnProjectile, nullptr, false, true
		);

		WaitGameplayEvent->EventReceived.AddDynamic(this, &ThisClass::SpawnProjectile);
		WaitGameplayEvent->ReadyForActivation();
	}
}

void URPGGA_Player_Nova::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URPGGA_Player_Nova::SpawnProjectile(FGameplayEventData PayloadData)
{
	FVector CharacterLocation = GetPlayerCharacterFromActorInfo()->GetActorLocation();
	FVector CharacterUp = GetPlayerCharacterFromActorInfo()->GetActorUpVector();
	
	float WeaponDamage = GetPlayerCharacterFromActorInfo()->GetCombatComponent()->GetCharacterCurrentEquippedWeapon()->WeaponDefaultData.WeaponBaseDamage;
	
	for (int32 i = 0; i < ProjectileCount; i++)
	{
		float Angle = (360.0f / ProjectileCount) * i;
		float AngleRadians = FMath::DegreesToRadians(Angle);
		
		FVector NearBodyPosition = FVector(
			FMath::Cos(AngleRadians) * StartRadius,
			FMath::Sin(AngleRadians) * StartRadius,
			0.0f
		);
		
		FVector ProjectileLocation = CharacterLocation + NearBodyPosition + (CharacterUp * ProjectileHeight);
		FVector FireDirection = NearBodyPosition.GetSafeNormal();
		FRotator ProjectileDirection = UKismetMathLibrary::MakeRotFromX(FireDirection);
	
		APooledActor* PooledActor = GetPlayerCharacterFromActorInfo()->GetObjectPoolComponent()->SpawnFromPool(RPGGameplayTag::Data_ObjectPoolType_Nova, ProjectileLocation, ProjectileDirection);
		if (AProjectileBase* Projectile = Cast<AProjectileBase>(PooledActor))
		{
			Projectile->LaunchProjectile(FireDirection, 700.0f);
			Projectile->DamageEffectSpecHandle = MakePlayerSkillDamageEffectSpecHandle(DamageEffectClass, WeaponDamage, DamageScale);
		}
	}
}
