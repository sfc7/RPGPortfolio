// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/RPGGA_Player_LightAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerAttributeSet.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Component/Player/PlayerCombatComponent.h"
#include "GameAbilitySystem/GameplayTask/Player/RPGAT_Player_RotateTarget.h"
#include "WorldStatic/Weapon/PlayerWeapon.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"

URPGGA_Player_LightAttack::URPGGA_Player_LightAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Player_LightAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 콤보 카운트 초기화
	GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);

	// 타겟 회전 필요 여부 확인 - 주변 적 확인
	const bool bRotate = FindNearestEnemyBeforeAttack(1000.f) && !HasMatchingGameplayTag(RPGGameplayTag::Player_Status_LockOnTarget); 

	// 여부에 따라 회전 후 공격 or 바로 공격
	if (bRotate)
	{
		URPGAT_Player_RotateTarget* RotateTickTask = URPGAT_Player_RotateTarget::ExecuteTaskOnTick(this);
		RotateTickTask->OnRotateTargetTaskTick.AddDynamic(this, &UPlayerCombatGameplayAbility::RotateTargetTickBeforeAttack);
		RotateTickTask->SetTargetRotation(FindRotation);
		RotateTickTask->OnRotationCompleted.AddDynamic(this, &URPGGA_Player_LightAttack::Attack);
		RotateTickTask->ReadyForActivation();
	}
	else
	{	
		Attack();
	}
}

void URPGGA_Player_LightAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URPGGA_Player_LightAttack::Attack()
{
	UPlayerCombatComponent* CombatComponent = GetPlayerCombatComponentFromActorInfo();
	if (!IsValid(CombatComponent)) return;

	AWeaponBase* CurrentWeapon = CombatComponent->GetCharacterCurrentEquippedWeapon();
	if (!IsValid(CurrentWeapon)) return;

	// 무기 공격 타입을 Light로 설정
	CurrentWeapon->SetCurrentAttackType(EWeaponAttackType::Light);

	// 현재 콤보 카운트를 ASC에서 가져오고 저장함 
	const float CurrentComboCount  = GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(UPlayerAttributeSet::GetCurrentLightAttackComboAttribute());
	UsedComboCount = CurrentComboCount;

	// 콤보 카운트에 따른 약 공격 애니메이션 선택
	UAnimMontage* MontageToPlay = nullptr;
	if(LightAttackMontages.Contains(CurrentComboCount))
	{
		MontageToPlay = LightAttackMontages[CurrentComboCount];
	}

	// 약 공격 애니메이션 재생 AbilityTask 생성
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,TEXT("Player_Light_Attack"), MontageToPlay, 1.0f,  NAME_None,
		true, 1.0f, false);

	PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Player_LightAttack::OnEndAbilityCallback);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Player_LightAttack::OnEndAbilityCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Player_LightAttack::OnEndAbilityCallback);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Player_LightAttack::OnEndAbilityCallback);
	PlayMontageTask->ReadyForActivation();

	// 애니메이션으로 부터 GameplayEvent를 기다림, 여기에는 데미지 계산, 이펙트 등의 로직을 바인딩
	UAbilityTask_WaitGameplayEvent* GameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,RPGGameplayTag::Character_Event_AttackHit_Melee,nullptr, false, true);

	GameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::ApplyEffectsSpecHandleToTargetCallback);
	GameplayEventTask->ReadyForActivation();

	// 콤보 카운트에 따라 초기화, 강 공격 어빌리티 가능 Tag 부여등 로직 처리
	if (CurrentComboCount == LightAttackMontages.Num())
	{
		ResetComboCount();
	}
	else
	{
		AddGameplayTag(GetPlayerCharacterFromActorInfo(), RPGGameplayTag::Player_Status_CanHeavyAttackCombo);
		GetAbilitySystemComponentFromActorInfo()->SetNumericAttributeBase(UPlayerAttributeSet::GetCurrentLightAttackComboAttribute(), CurrentComboCount + 1);
	}
}

void URPGGA_Player_LightAttack::ResetComboCount()
{
	// 콤보 카운트 제거 및 리셋
	GetAbilitySystemComponentFromActorInfo()->SetNumericAttributeBase(UPlayerAttributeSet::GetCurrentLightAttackComboAttribute(), 1);
	RemoveGameplayTag(GetPlayerCharacterFromActorInfo(), RPGGameplayTag::Player_Status_CanHeavyAttackCombo);
}

void URPGGA_Player_LightAttack::ResetComboCountTimer()
{
	// 콤보 카운트 리셋
	GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &URPGGA_Player_LightAttack::ResetComboCount, 0.5f, false);
}

void URPGGA_Player_LightAttack::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

	ResetComboCountTimer();
}

void URPGGA_Player_LightAttack::ApplyEffectsSpecHandleToTargetCallback(FGameplayEventData PayloadData)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!IsValid(ASC)) return;
	
	// PayloadData에서 가져와 타겟 액터 설정
	LocalTargetActor = const_cast<AActor*>(PayloadData.Target.Get());

	// PayloadData에서 무기 공격률 및 기본 데미지 가져오기
	float WeaponAttackRate = PayloadData.EventMagnitude;
	float WeaponBaseDamage = GetPlayerCharacterFromActorInfo()->GetCombatComponent()->GetCharacterCurrentEquippedWeapon()->WeaponDefaultData.WeaponBaseDamage;

	// 콤보 데미지 GameplayEffectSpec 생성
	FGameplayEffectSpecHandle SpecHandle = MakePlayerComboDamageEffectSpecHandle(DamageEffectClass, WeaponAttackRate, WeaponBaseDamage, RPGGameplayTag::Data_DamageType_SetByCaller_Light, UsedComboCount);
	// GameplayEffectContext를 만들어서 GameplayCue로 약 공격 이펙트 처리
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	ASC->ExecuteGameplayCue(RPGGameplayTag::GameplayCue_Player_Fighter_Sound_AttackHit_Melee_Light, EffectContext);
	
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
	GetPlayerCharacterFromActorInfo()->GetRPGAbilitySystemComponent()->ExecuteGameplayCue(RPGGameplayTag::GameplayCue_Player_Fighter_Effect_AttackHit_Melee_Light, AttackHitGCParam);
}
