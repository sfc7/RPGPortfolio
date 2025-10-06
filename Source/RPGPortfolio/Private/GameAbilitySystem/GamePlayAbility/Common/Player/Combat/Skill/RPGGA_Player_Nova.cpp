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

	//@ 마나 소모 및 쿨다운 적용
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	if (GetPlayerCharacterFromActorInfo()->GetPlayerSkillComponent())
	{
		// 퀵슬롯에 해당 스킬이 등록되어 있다면 쿨다운 UI 표시
		int32 QuickSlotIndex = GetPlayerCharacterFromActorInfo()->GetPlayerSkillComponent()->FindQuickSlotIndexByTag(RPGGameplayTag::Player_Ability_Skill_Nova);
        
		GetPlayerCharacterFromActorInfo()->GetPlayerUIComponent()->OnSkillCooldownBeginDelegate.Broadcast(
			QuickSlotIndex, 
			RPGGameplayTag::Player_Ability_Skill_Nova, 
			GetCooldownTimeRemaining(), 
			GetCooldownTimeRemaining()
		);
	}
	
	Attack();
}

void URPGGA_Player_Nova::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URPGGA_Player_Nova::Attack()
{
	// Nova 애니메이션 재생 AbilityTask 생성
	if (IsValid(AttackMontage))
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
	this,TEXT("Player_Skill_Nova"), AttackMontage, 1.0f,  NAME_None,
	true, 1.0f, false);

		PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Player_Nova::OnEndAbilityCallback);
		PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Player_Nova::OnEndAbilityCallback);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Player_Nova::OnEndAbilityCallback);
		PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Player_Nova::OnEndAbilityCallback);
		PlayMontageTask->ReadyForActivation();

		// 애니메이션으로 부터 GameplayEvent를 기다림, 여기에는 발사체 생성을 바인딩
		UAbilityTask_WaitGameplayEvent* WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, RPGGameplayTag::Character_Event_SpawnProjectile, nullptr, false, true
		);

		WaitGameplayEvent->EventReceived.AddDynamic(this, &ThisClass::SpawnProjectile);
		WaitGameplayEvent->ReadyForActivation();
	}
}

void URPGGA_Player_Nova::SpawnProjectile(FGameplayEventData PayloadData)
{
	// 발사체 생성 위치 
	const FVector CharacterLocation = GetPlayerCharacterFromActorInfo()->GetActorLocation();
	const FVector CharacterUp = GetPlayerCharacterFromActorInfo()->GetActorUpVector();
	
	const float WeaponDamage = GetPlayerCharacterFromActorInfo()->GetCombatComponent()->GetCharacterCurrentEquippedWeapon()->WeaponDefaultData.WeaponBaseDamage;

	// ProjectileCount 만큼 360도 발사하는 로직
	for (int32 i = 0; i < ProjectileCount; i++)
	{
		float Angle = (360.0f / ProjectileCount) * i;
		float AngleRadians = FMath::DegreesToRadians(Angle);

		// 반지름과 라디안을 통해 반지름 거리 만큼 원형으로 ProjectileCount개수의 좌표 생성
		FVector NearBodyPosition = FVector(
			FMath::Cos(AngleRadians) * StartRadius,
			FMath::Sin(AngleRadians) * StartRadius,
			0.0f
		);

		// 발사체 생성 초기회전
		const FVector ProjectileLocation = CharacterLocation + NearBodyPosition + (CharacterUp * ProjectileHeight);
		const FVector FireDirection = NearBodyPosition.GetSafeNormal();
		const FRotator ProjectileDirection = UKismetMathLibrary::MakeRotFromX(FireDirection);

		// 오브젝트풀 패턴을 통한 발사체 생성
		APooledActor* PooledActor = GetPlayerCharacterFromActorInfo()->GetObjectPoolComponent()->SpawnFromPool(RPGGameplayTag::Data_ObjectPoolType_Nova, ProjectileLocation, ProjectileDirection);
		if (AProjectileBase* Projectile = Cast<AProjectileBase>(PooledActor))
		{
			Projectile->LaunchProjectile(FireDirection, 700.0f);
			Projectile->DamageEffectSpecHandle = MakePlayerSkillDamageEffectSpecHandle(DamageEffectClass, WeaponDamage, DamageScale);
		}
	}
}
