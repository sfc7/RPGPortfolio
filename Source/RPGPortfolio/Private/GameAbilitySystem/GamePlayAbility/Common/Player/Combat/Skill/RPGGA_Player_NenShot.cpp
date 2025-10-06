// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/Skill/RPGGA_Player_NenShot.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "WorldStatic/Projectile/ProjectileBase.h"
#include "Component/CombatComponentBase.h"
#include "Component/Player/ObjectPoolComponent.h"
#include "WorldStatic/Weapon/WeaponBase.h"
#include "Component/Player/PlayerSkillComponent.h"
#include "Component/Player/PlayerUIComponent.h"
#include "GameAbilitySystem/GameplayTask/Player/RPGAT_Player_RotateTarget.h"

URPGGA_Player_NenShot::URPGGA_Player_NenShot()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Player_NenShot::ActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//@ 마나 소모 및 쿨다운 적용
	CommitAbility(Handle, ActorInfo, ActivationInfo);
	
	if (GetPlayerCharacterFromActorInfo()->GetPlayerSkillComponent())
	{
		// 퀵슬롯에 해당 스킬이 등록되어 있다면 쿨다운 UI 표시
		int32 QuickSlotIndex = GetPlayerCharacterFromActorInfo()->GetPlayerSkillComponent()->FindQuickSlotIndexByTag(RPGGameplayTag::Player_Ability_Skill_NenShot);
        
		GetPlayerCharacterFromActorInfo()->GetPlayerUIComponent()->OnSkillCooldownBeginDelegate.Broadcast(
			QuickSlotIndex, 
			RPGGameplayTag::Player_Ability_Skill_NenShot, 
			GetCooldownTimeRemaining(), 
			GetCooldownTimeRemaining()
		);
		
	}

	// 타겟 회전 필요 여부 확인 - 주변 적 확인
	bool bRotate = FindNearestEnemyBeforeAttack(1000.f) && !HasMatchingGameplayTag(RPGGameplayTag::Player_Status_LockOnTarget); 

	// 여부에 따라 회전 후 공격 or 바로 공격
	if (bRotate)
	{
		URPGAT_Player_RotateTarget* RotateTickTask = URPGAT_Player_RotateTarget::ExecuteTaskOnTick(this);
		RotateTickTask->OnRotateTargetTaskTick.AddDynamic(this, &UPlayerCombatGameplayAbility::RotateTargetTickBeforeAttack);
		RotateTickTask->SetTargetRotation(FindRotation);
		RotateTickTask->OnRotationCompleted.AddDynamic(this, &URPGGA_Player_NenShot::Attack);
		RotateTickTask->ReadyForActivation();
	}
	else
	{	
		Attack();
	}
}

void URPGGA_Player_NenShot::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URPGGA_Player_NenShot::Attack()
{
	// NenShot 애니메이션 재생 AbilityTask 생성
	if (IsValid(AttackMontage))
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
	this,TEXT("Player_Skill_NenShot"), AttackMontage, 1.0f,  NAME_None,
	true, 1.0f, false);

		PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Player_NenShot::OnEndAbilityCallback);
		PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Player_NenShot::OnEndAbilityCallback);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Player_NenShot::OnEndAbilityCallback);
		PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Player_NenShot::OnEndAbilityCallback);
		PlayMontageTask->ReadyForActivation();

		// 애니메이션으로 부터 GameplayEvent를 기다림, 여기에는 발사체 생성을 바인딩
		UAbilityTask_WaitGameplayEvent* WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, RPGGameplayTag::Character_Event_SpawnProjectile, nullptr, false, true
		);

		WaitGameplayEvent->EventReceived.AddDynamic(this, &ThisClass::SpawnProjectile);
		WaitGameplayEvent->ReadyForActivation();
	}
}

void URPGGA_Player_NenShot::SpawnProjectile(FGameplayEventData PayloadData)
{
	// 발사체 생성 위치 
	const FVector CharacterLocation = GetPlayerCharacterFromActorInfo()->GetActorLocation();
	const FVector CharacterForward = GetPlayerCharacterFromActorInfo()->GetActorForwardVector();
	const FVector CharacterRight = GetPlayerCharacterFromActorInfo()->GetActorRightVector();
	const FVector CharacterUp = GetPlayerCharacterFromActorInfo()->GetActorUpVector();
	const FVector ProjectileLocation = CharacterLocation + (CharacterForward * 50.0f) + (CharacterUp * 50.0f);

	// 발사체 생성 초기회전
	FRotator ProjectileDirection = UKismetMathLibrary::MakeRotFromX(GetPlayerCharacterFromActorInfo()->GetActorForwardVector());

	// 오브젝트풀 패턴을 통한 발사체 생성
	APooledActor* PooledActor = GetPlayerCharacterFromActorInfo()->GetObjectPoolComponent()->SpawnFromPool(RPGGameplayTag::Data_ObjectPoolType_NenShot,ProjectileLocation, ProjectileDirection);
	if (AProjectileBase* Projectile = Cast<AProjectileBase>(PooledActor))
	{
		Projectile->LaunchProjectile(GetPlayerCharacterFromActorInfo()->GetActorForwardVector(), 700.0f);
		const float WeaponDamage = GetPlayerCharacterFromActorInfo()->GetCombatComponent()->GetCharacterCurrentEquippedWeapon()->WeaponDefaultData.WeaponBaseDamage;
		Projectile->DamageEffectSpecHandle = MakePlayerSkillDamageEffectSpecHandle(DamageEffectClass, WeaponDamage, DamageScale);
	}
}
