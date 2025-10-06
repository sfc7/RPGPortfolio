// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/Skill/RPGGA_Player_TeleportDropKick.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "MotionWarpingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerAttributeSet.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "Component/Player/PlayerCombatComponent.h"
#include "WorldStatic/Weapon/PlayerWeapon.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "Component/Player/PlayerUIComponent.h"
#include "Component/Player/PlayerSkillComponent.h"

URPGGA_Player_TeleportDropKick::URPGGA_Player_TeleportDropKick()
{
}

void URPGGA_Player_TeleportDropKick::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	APlayerCharacterBase* OwnerCharacter = Cast<APlayerCharacterBase>(ActorInfo->AvatarActor.Get());
	if (!IsValid(OwnerCharacter)) return;

	// 캐릭터 MomvementMode를 Walking으로 설정
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	//@ 마나 소모 및 쿨다운 적용
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	if (GetPlayerCharacterFromActorInfo()->GetPlayerSkillComponent())
	{
		// 퀵슬롯에 해당 스킬이 등록되어 있다면 쿨다운 UI 표시
		int32 QuickSlotIndex = GetPlayerCharacterFromActorInfo()->GetPlayerSkillComponent()->FindQuickSlotIndexByTag(RPGGameplayTag::Player_Ability_Skill_DropKick);
        
		GetPlayerCharacterFromActorInfo()->GetPlayerUIComponent()->OnSkillCooldownBeginDelegate.Broadcast(
			QuickSlotIndex, 
			RPGGameplayTag::Player_Ability_Skill_DropKick, 
			GetCooldownTimeRemaining(),		
			GetCooldownTimeRemaining()
		);
	}

	// 타겟 회전 필요 여부 확인 - 주변 적 확인
	const bool bRotate = FindNearestEnemyBeforeAttack(1000.f) && !HasMatchingGameplayTag(RPGGameplayTag::Player_Status_LockOnTarget); 

	// 타겟 여부에 따라 회전 후 공격 or 바로 공격
	if (IsValid(RotateTargetActor))
	{
		// 타겟 여부에 따라 일직선 방향 실행, 회전하여 실행
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(RotateTargetActor);
		if (TargetASC && !TargetASC->HasMatchingGameplayTag(RPGGameplayTag::Character_Status_Dead))
		{
			RotateDirection();
		}
		else
		{
			ForwardDirection();
		}
	}
	else
	{
		ForwardDirection();
	}

	// 드롭킥 공격 애니메이션 재생 AbilityTask 생성
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
		TEXT("Player_TeleportDropKick"), AttackMontage, 1.0f, NAME_None,
		true, 1.0f, false);
    
	PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Player_TeleportDropKick::OnEndAbilityCallback);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Player_TeleportDropKick::OnEndAbilityCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Player_TeleportDropKick::OnEndAbilityCallback);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Player_TeleportDropKick::OnEndAbilityCallback);
	PlayMontageTask->ReadyForActivation();

	// 애니메이션으로 부터 GameplayEvent를 기다림, 여기에는 데미지 계산, 이펙트 등의 로직을 바인딩
	UAbilityTask_WaitGameplayEvent* GameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, RPGGameplayTag::Character_Event_AttackHit_Melee, nullptr, false, true);

	GameplayEventTask->EventReceived.AddDynamic(this, &URPGGA_Player_TeleportDropKick::ApplyEffectsSpecHandleToTargetCallback);
	GameplayEventTask->ReadyForActivation();
    
	CommitAbility(Handle, ActorInfo, ActivationInfo);
}

void URPGGA_Player_TeleportDropKick::ForwardDirection()
{
	if (WarpTargetNameDistance == FName(TEXT("None")) || WarpTargetNameDistance == FName(TEXT(""))) return;

	// 일직선 방향으로 500.f로 정해진 값 모션 워프
	const FVector MyLocation = GetPlayerCharacterFromActorInfo()->GetActorLocation();
	const FVector MyForward = GetPlayerCharacterFromActorInfo()->GetActorForwardVector();
	const FVector WarpLocation = MyLocation + (MyForward * 500.0f);
    
	GetPlayerCharacterFromActorInfo()->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(WarpTargetNameDistance, WarpLocation);
}

void URPGGA_Player_TeleportDropKick::RotateDirection()
{
	if (IsValid(RotateTargetActor))
	{
		// 타겟 방향으로 모션워핑 과정
		const FVector TargetLocation = RotateTargetActor->GetActorLocation();
		const FVector MyLocation = GetPlayerCharacterFromActorInfo()->GetActorLocation();
		const FVector DirectionToTarget = (TargetLocation - MyLocation).GetSafeNormal();
 
		const FVector WarpLocation = TargetLocation - (DirectionToTarget * 50.0f);
		const FRotator WarpRotation = UKismetMathLibrary::FindLookAtRotation(WarpLocation, TargetLocation);

		GetPlayerCharacterFromActorInfo()->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(
		   WarpTargetNameDirection, FVector::ZeroVector, FRotationMatrix::MakeFromX(DirectionToTarget).Rotator());
		
		GetPlayerCharacterFromActorInfo()->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(WarpTargetNameDistance, WarpLocation);
	}	
}

void URPGGA_Player_TeleportDropKick::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URPGGA_Player_TeleportDropKick::ApplyEffectsSpecHandleToTargetCallback(FGameplayEventData PayloadData)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!IsValid(ASC)) return;
	
	// PayloadData에서 가져와 타겟 액터 설정
	AActor* LocalTargetActor = const_cast<AActor*>(PayloadData.Target.Get());

	// PayloadData에서 무기 공격률 및 기본 데미지 가져오기
	float WeaponDamage = GetPlayerCharacterFromActorInfo()->GetCombatComponent()->GetCharacterCurrentEquippedWeapon()->WeaponDefaultData.WeaponBaseDamage;
	FGameplayEffectSpecHandle SpecHandle = MakePlayerSkillDamageEffectSpecHandle(DamageEffectClass, WeaponDamage, DamageScale);

	// GameplayEffectContext를 만들어서 GameplayCue로 드롭킥 공격 이펙트 처리
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	ASC->ExecuteGameplayCue(RPGGameplayTag::GameplayCue_Player_Fighter_Sound_AttackHit_Melee_Heavy, EffectContext);

	// GameplayEffectSpec 적용 후 GameplayEffect 생성, 성공 시 대상에게 경직 효과 적용
	FActiveGameplayEffectHandle HitReactGameplayEffectHandle = ApplyEffectsSpecHandleToTarget(LocalTargetActor, SpecHandle);
	if (HitReactGameplayEffectHandle.WasSuccessfullyApplied())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(LocalTargetActor, RPGGameplayTag::Character_Event_HitReact, PayloadData);
	}

	// 공격 적중 이펙트 GameplayCue 실행
	FVector AttackDirection = (LocalTargetActor->GetActorLocation() - GetPlayerCharacterFromActorInfo()->GetActorLocation()).GetSafeNormal();
	FVector HitLocation = PayloadData.ContextHandle.GetHitResult()->Location;
	FGameplayCueParameters AttackHitGCParam;
	AttackHitGCParam.Normal = AttackDirection;
	AttackHitGCParam.TargetAttachComponent = GetOwningComponentFromActorInfo();
	AttackHitGCParam.Location = HitLocation;	
	GetPlayerCharacterFromActorInfo()->GetRPGAbilitySystemComponent()->ExecuteGameplayCue(RPGGameplayTag::GameplayCue_Player_Fighter_Effect_AttackHit_Melee_Heavy, AttackHitGCParam);
}
