// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/RPGGA_Player_ParryingAttack.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Component/Player/PlayerCombatComponent.h"
#include "RPGStructTypes.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "WorldStatic/Weapon/WeaponBase.h"
#include "GameAbilitySystem/GameplayTask/Player/RPGAT_Player_RotateTarget.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"

URPGGA_Player_ParryingAttack::URPGGA_Player_ParryingAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Player_ParryingAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 타겟 회전 필요 여부 확인 - 주변 적 확인
	const bool bRotate = FindNearestEnemyBeforeAttack(1000.f) && !HasMatchingGameplayTag(RPGGameplayTag::Player_Status_LockOnTarget); 

	// 여부에 따라 회전 후 공격 or 바로 공격
	if (bRotate)
	{		
		URPGAT_Player_RotateTarget* RotateTickTask = URPGAT_Player_RotateTarget::ExecuteTaskOnTick(this);
		RotateTickTask->OnRotateTargetTaskTick.AddDynamic(this, &UPlayerCombatGameplayAbility::RotateTargetTickBeforeAttack);
		RotateTickTask->SetTargetRotation(FindRotation);
		RotateTickTask->OnRotationCompleted.AddDynamic(this, &URPGGA_Player_ParryingAttack::Attack);
		RotateTickTask->ReadyForActivation();
	}
	else
	{	
		Attack();
	}
}

void URPGGA_Player_ParryingAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URPGGA_Player_ParryingAttack::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URPGGA_Player_ParryingAttack::Attack()
{
	UPlayerCombatComponent* CombatComponent = GetPlayerCombatComponentFromActorInfo();
	if (!IsValid(CombatComponent)) return;

	AWeaponBase* CurrentWeapon = CombatComponent->GetCharacterCurrentEquippedWeapon();
	if (!IsValid(CurrentWeapon)) return;

	// 무기 공격 타입을 Parrying로 설정
	CurrentWeapon->SetCurrentAttackType(EWeaponAttackType::Parrying);

	// 패링 공격 애니메이션 재생 AbilityTask 생성
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,TEXT("Player_Parrying_Attack"), ParryingAttackMontage, 1.0f,  NAME_None,
		true, 1.0f, false);

	PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Player_ParryingAttack::OnEndAbilityCallback);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Player_ParryingAttack::OnEndAbilityCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Player_ParryingAttack::OnEndAbilityCallback);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Player_ParryingAttack::OnEndAbilityCallback);
	PlayMontageTask->ReadyForActivation();

	// 애니메이션으로 부터 GameplayEvent를 기다림, 여기에는 데미지 계산, 이펙트 등의 로직을 바인딩
	UAbilityTask_WaitGameplayEvent* AttackHitGE = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this,RPGGameplayTag::Character_Event_AttackHit_Melee,nullptr, false, true);

	AttackHitGE->EventReceived.AddDynamic(this, &ThisClass::ApplyEffectsSpecHandleToTargetCallback);
	AttackHitGE->ReadyForActivation();
}

void URPGGA_Player_ParryingAttack::ApplyEffectsSpecHandleToTargetCallback(FGameplayEventData PayloadData)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!IsValid(ASC)) return;
	
	// PayloadData에서 가져와 타겟 액터 설정	
	LocalTargetActor = const_cast<AActor*>(PayloadData.Target.Get());

	// PayloadData에서 무기 공격률 및 기본 데미지 가져오기
	float WeaponAttackRate = PayloadData.EventMagnitude;
	float WeaponBaseDamage = GetPlayerCharacterFromActorInfo()->GetCombatComponent()->GetCharacterCurrentEquippedWeapon()->WeaponDefaultData.WeaponBaseDamage;

	// 콤보 데미지 GameplayEffectSpec 생성
	FGameplayEffectSpecHandle SpecHandle = MakePlayerBaseDamageEffectSpecHandle(DamageEffectClass, WeaponAttackRate, WeaponBaseDamage, RPGGameplayTag::Data_DamageType_SetByCaller_Parrying);
	// GameplayEffectContext를 만들어서 GameplayCue로 약 공격 이펙트 처리
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	ASC->ExecuteGameplayCue(RPGGameplayTag::GameplayCue_Player_Fighter_Sound_AttackHit_Parrying, EffectContext);
	
	// GameplayEffectSpec 적용 후 GameplayEffect 생성, 성공 시 대상에게 경직 효과 적용
	FActiveGameplayEffectHandle HitReactGameplayEffectHandle = ApplyEffectsSpecHandleToTarget(LocalTargetActor, SpecHandle);
	if (HitReactGameplayEffectHandle.WasSuccessfullyApplied())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(LocalTargetActor, RPGGameplayTag::Character_Event_HitReact, PayloadData);
	}

	// 타격 성공 시 플레이어에게 마나 획득 적용
	BP_ApplyGameplayEffectToOwner(GainMpEffectClass,GetAbilityLevel());

	// 공격 적중 이펙트 GameplayCue 실행
	FVector AttackDirection = (LocalTargetActor->GetActorLocation() - GetPlayerCharacterFromActorInfo()->GetActorLocation()).GetSafeNormal();
	FVector HitLocation = PayloadData.ContextHandle.GetHitResult()->Location;
	FGameplayCueParameters AttackHitGCParam;
	AttackHitGCParam.Normal = AttackDirection;
	AttackHitGCParam.TargetAttachComponent = GetOwningComponentFromActorInfo();
	AttackHitGCParam.Location = HitLocation;	
	GetPlayerCharacterFromActorInfo()->GetRPGAbilitySystemComponent()->ExecuteGameplayCue(RPGGameplayTag::GameplayCue_Player_Fighter_Effect_AttackHit_Parrying, AttackHitGCParam);
}
