// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/RPGGA_Player_Defense.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BlueprintGameplayTagLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "GameMode/GameManager/CombatManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

URPGGA_Player_Defense::URPGGA_Player_Defense()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Player_Defense::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 전투 매니저 캐시 및 설정값 가져오기
	CachedCombatManager = GetWorld()->GetGameInstance()->GetSubsystem<UCombatManager>();
	CanParryingAttackStateDelayTime = CachedCombatManager->GetAdjustedCanParryingStateDelay();
	ParryingAttackASCActiveTime = CachedCombatManager->GetAdjustedParryingASCActiveTime();
	ParryingSuccessCheckTime = CachedCombatManager->GetAdjustedParryingASCActiveTime() * 2;
	DefenseActivateTime = GetWorld()->GetTimeSeconds();

	// 패링 성공 체크 태그 추가(패링 성공여부 체크용)
	AddGameplayTag(GetPlayerCharacterFromActorInfo(), RPGGameplayTag::Player_Status_ParryingSuccessCheck); 

	// 패링 입력 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(ParryingInputTimerHandle, [this]()
	{
		RemoveGameplayTag(GetPlayerCharacterFromActorInfo(), RPGGameplayTag::Player_Status_ParryingSuccessCheck);
	}, ParryingSuccessCheckTime, false);

	// 디펜스 Montage 실행 및 바인딩
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
	TEXT("Player_Defense"), DefenseMontage, 1.0f, NAME_None,
	true, 1.0f, false);
	
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Player_Defense::OnEndAbilityCallback);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Player_Defense::OnEndAbilityCallback);
	PlayMontageTask->ReadyForActivation();

	// 디펜스 이펙트 관련 처리
	FGameplayCueParameters DefenseGCEffectParam;
	DefenseGCEffectParam.TargetAttachComponent = GetOwningComponentFromActorInfo();
	GetPlayerCharacterFromActorInfo()->GetRPGAbilitySystemComponent()->AddGameplayCue(DefenseEffectGamePlayCue, DefenseGCEffectParam);

	UAbilityTask_WaitGameplayEvent* DefenseSuccessGE = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this, RPGGameplayTag::Player_Event_DefenseSuccess, nullptr, false, true
	);
	DefenseSuccessGE->EventReceived.AddDynamic(this, &URPGGA_Player_Defense::SuccessDefenseCallback);
	DefenseSuccessGE->ReadyForActivation();
}

void URPGGA_Player_Defense::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// 글로벌 시간 지연 복원
	if (UGameplayStatics::GetGlobalTimeDilation(GetWorld()) != 1.0f)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	}

	// 디펜스 이펙트 GameplayCue 제거
	GetPlayerCharacterFromActorInfo()->GetRPGAbilitySystemComponent()->RemoveGameplayCue(DefenseEffectGamePlayCue);
}

void URPGGA_Player_Defense::SuccessDefenseCallback(FGameplayEventData PayloadData)
{
	// 플레이어 위치 및 방향 벡터 계산
	const FVector PlayerLocation = GetPlayerCharacterFromActorInfo()->GetActorLocation();
	const FVector PlayerBackVector = GetPlayerCharacterFromActorInfo()->GetActorForwardVector() * -1;
	const FVector TargetLocation = PayloadData.Instigator->GetActorLocation();

	// 플레이어가 뒤로 밀려나는 루트 모션 적용
	UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce( this,                         
		TEXT("None"),      
		PlayerBackVector,         
		150.0f,                        
		0.2f,                      
		false,                      
		nullptr,                    
		ERootMotionFinishVelocityMode::MaintainLastRootMotionVelocity,
		FVector::ZeroVector,         
		0.0f, false);

	// 패링이 가능한 시간을 넘었는지에 관한 체크
	const bool bIsParrying = (GetWorld()->GetTimeSeconds() - DefenseActivateTime) <= CanParryingAttackStateDelayTime;

	// 패링 성공 시
	if(bIsParrying)
	{
		if (HasMatchingGameplayTag(RPGGameplayTag::Player_Status_ParryingSuccessCheck))
		{
			CachedCombatManager->RecordParryAttempt(true);
		}

		// 패링 이펙트 및 연출
		FGameplayCueParameters DefenseParryingGCParam;
		DefenseParryingGCParam.TargetAttachComponent = GetOwningComponentFromActorInfo();
		GetPlayerCharacterFromActorInfo()->GetRPGAbilitySystemComponent()->ExecuteGameplayCue(DefenseParryingGamePlayCue, DefenseParryingGCParam);
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), GlobalTimeDilation);

		// 패링 어택 관련 로직
		SetParryingAttackReady();
		RemoveParryingAttackReady();
		GetWorld()->GetTimerManager().SetTimer(ParryingInputTimerHandle, [this]()
		{
			RemoveGameplayTag(GetPlayerCharacterFromActorInfo(), RPGGameplayTag::Player_Status_CanParryingAttack);
		}, CanParryingAttackStateDelayTime, false);
	}
	else
	{
		// 로그에 실패 기록
		if (HasMatchingGameplayTag(RPGGameplayTag::Player_Status_ParryingSuccessCheck))
		{
			CachedCombatManager->RecordParryAttempt(false);
		}
		// 패링이 실패했음으로 디펜스에 관한 연출 
		FGameplayCueParameters DefenseSucessGCParam;
		DefenseSucessGCParam.TargetAttachComponent = GetOwningComponentFromActorInfo();
		GetPlayerCharacterFromActorInfo()->GetRPGAbilitySystemComponent()->ExecuteGameplayCue(DefenseSuccessGamePlayCue, DefenseSucessGCParam);
	}
}

void URPGGA_Player_Defense::SetParryingAttackReady()
{
	// 패링 공격용 IMC GameplayTag 추가
	AddGameplayTag(GetPlayerCharacterFromActorInfo(), RPGGameplayTag::Player_Status_CanParryingAttack); 

	// 패링 어택 어빌리티 부여
	FGameplayAbilitySpec ParryAbilitySpec(ParryingAttackGA);
	ParryAbilitySpec.SourceObject = GetPlayerCharacterFromActorInfo();
	ParryAbilitySpec.Level = GetAbilityLevel();
	ParryAbilitySpec.GetDynamicSpecSourceTags().AddTag(ParryingInputTag);
	ParryAbilityHandle = GetRPGAbilitySystemComponentFromActorInfo()->GiveAbility(ParryAbilitySpec);
}

void URPGGA_Player_Defense::RemoveParryingAttackReady()
{
	// 글로벌 지연 효과 (패링 공격의 연출용)
	GetWorld()->GetTimerManager().SetTimer(ParryingDelayAndGATimerHandle, [this]()
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	}, 0.2f, false);

	// 패링 어빌리티 제거 타이머 설정
	if (ParryAbilityHandle.IsValid())
	{
		FGameplayAbilitySpec* Spec = GetRPGAbilitySystemComponentFromActorInfo()->FindAbilitySpecFromHandle(ParryAbilityHandle);
		if (Spec)
		{
			GetWorld()->GetTimerManager().SetTimer(ParryingDelayAndGATimerHandle, [this]()
			{
				GetRPGAbilitySystemComponentFromActorInfo()->ClearAbility(ParryAbilityHandle);
				ParryAbilityHandle = FGameplayAbilitySpecHandle(); 
			}, ParryingAttackASCActiveTime, false);
		}
	}
}

void URPGGA_Player_Defense::OnEndAbilityCallback()
{
	RemoveParryingAttackReady();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
