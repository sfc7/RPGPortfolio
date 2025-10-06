// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Monster/RPGGA_Monster_Attack_EarthQuake.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "MeshPaintVisualize.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/MonsterCharacter.h"
#include "Kismet/KismetMathLibrary.h"

URPGGA_Monster_Attack_EarthQuake::URPGGA_Monster_Attack_EarthQuake()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Monster_Attack_EarthQuake::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 지진 패턴 애니메이션 재생 AbilityTask 생성
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
	this,TEXT("Player_Attack_EarthQuake"), AttackMontage, 1.0f,  NAME_None,
true, 1.0f, false);

	PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Monster_Attack_EarthQuake::OnEndAbilityCallback);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Monster_Attack_EarthQuake::OnEndAbilityCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Monster_Attack_EarthQuake::OnEndAbilityCallback);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Monster_Attack_EarthQuake::OnEndAbilityCallback);
	PlayMontageTask->ReadyForActivation();

	// 애니메이션으로 부터 GameplayEvent를 기다림, 여기에는 실질적인 지진 패턴 발생, 데미지 계산, 이펙트 등의 로직을 바인딩
	UAbilityTask_WaitGameplayEvent* WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, RPGGameplayTag::Character_Event_AttackHit_AreaAttack, nullptr, false, true
	);

	WaitGameplayEvent->EventReceived.AddDynamic(this, &ThisClass::CheckBoxTraceAndApplyDamgage);
	WaitGameplayEvent->ReadyForActivation();
}

void URPGGA_Monster_Attack_EarthQuake::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URPGGA_Monster_Attack_EarthQuake::CheckBoxTraceAndApplyDamgage(FGameplayEventData PayloadData)
{
	// 지진 패턴에 사용할 BoxTrace 를 위한 초기 값
	const FVector StartLocation = GetMonsterCharacterFromActorInfo()->GetActorLocation();
	const FVector EndLocation = GetMonsterCharacterFromActorInfo()->GetActorForwardVector() * 100.f;
	const FRotator EndRotation = UKismetMathLibrary::MakeRotFromX(GetMonsterCharacterFromActorInfo()->GetActorForwardVector());
	// 지진 패턴을 적용할 오브젝트 타입설정
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> ActorsToIgnore;

	// 데미지를 적용할 적중한 HitResult들
	TArray<FHitResult> OUTHitResults;
	// 지진 패턴에 사용할 BoxTrace 생성
	UKismetSystemLibrary::BoxTraceMultiForObjects(this, StartLocation, EndLocation, FVector(EarthQuakeRadius, EarthQuakeRadius, 200.f), EndRotation,
		ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, OUTHitResults, true,FColor::Green, FColor::Red, 2.0f);

	// 데미지 계산 GameplayEffect를 HitResult들에 적용
	FGameplayEffectSpecHandle SpecHandle = MakeMonsterBaseDamageEffectSpecHandle(DamageEffectClass, DamageScale);
	ApplyGameplayEffectgSpecHandleToHitResults(SpecHandle, OUTHitResults);
}