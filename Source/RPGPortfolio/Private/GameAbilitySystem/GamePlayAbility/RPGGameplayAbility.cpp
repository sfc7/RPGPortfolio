// LJS


#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "Component/CombatComponentBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "RPGFunc.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"

void URPGGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	// OnGiven 정책인 경우 즉시 활성화
	if (AbilityActivationPolicy == ERPGGameplayAbilityActivationType::OnGiven)
	{
		if (ActorInfo && !Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void URPGGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// OnGiven 정책인 경우 즉시 제거
	if (AbilityActivationPolicy == ERPGGameplayAbilityActivationType::OnGiven)
	{
		if (ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

UCombatComponentBase* URPGGameplayAbility::GetCombatComponentFromActorInfo() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UCombatComponentBase>();
}

URPGAbilitySystemComponent* URPGGameplayAbility::GetRPGAbilitySystemComponentFromActorInfo() const
{
	return Cast<URPGAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

FActiveGameplayEffectHandle URPGGameplayAbility::ApplyEffectsSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& _SpecHandle)
{
	if (!IsValid(TargetActor)) return FActiveGameplayEffectHandle();

	UAbilitySystemComponent* const TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!IsValid(TargetASC)) return FActiveGameplayEffectHandle();

	if (!_SpecHandle.IsValid()) return FActiveGameplayEffectHandle();

	URPGAbilitySystemComponent* const RPGASC = GetRPGAbilitySystemComponentFromActorInfo();
	if (!IsValid(RPGASC)) return FActiveGameplayEffectHandle();
	
	// 타겟에게 게임플레이 이펙트 스펙 적용
	return RPGASC->ApplyGameplayEffectSpecToTarget(*_SpecHandle.Data, TargetASC);
}

void URPGGameplayAbility::AddGameplayTag(AActor* _Actor, FGameplayTag _GameplayTag)
{
	if (!IsValid(_Actor)) return;

	if (!_GameplayTag.IsValid()) return;

	URPGAbilitySystemComponent* const ASC = GetRPGAbilitySystemComponentFromActorInfo();
	if (!IsValid(ASC)) return;

	// 이미 태그가 있는지 확인 후 추가
	const bool bAlreadyHasTag = ASC->HasMatchingGameplayTag(_GameplayTag);
	if (!bAlreadyHasTag)
	{
		ASC->AddLooseGameplayTag(_GameplayTag);
	}
}

void URPGGameplayAbility::RemoveGameplayTag(AActor* _Actor, FGameplayTag _GameplayTag)
{
	if (!IsValid(_Actor)) return;

	if (!_GameplayTag.IsValid()) return;

	URPGAbilitySystemComponent* const ASC = GetRPGAbilitySystemComponentFromActorInfo();
	if (!IsValid(ASC)) return;

	// 태그가 있는지 확인 후 제거
	const bool bHasTag = ASC->HasMatchingGameplayTag(_GameplayTag);
	if (bHasTag)
	{
		ASC->RemoveLooseGameplayTag(_GameplayTag);
	}
}

bool URPGGameplayAbility::HasMatchingGameplayTag(FGameplayTag _GameplayTag)	
{
	if (!_GameplayTag.IsValid()) return false;

	URPGAbilitySystemComponent* const ASC = GetRPGAbilitySystemComponentFromActorInfo();
	if (!IsValid(ASC)) return false;

	return ASC->HasMatchingGameplayTag(_GameplayTag);
}

void URPGGameplayAbility::ApplyGameplayEffectgSpecHandleToHitResults(const FGameplayEffectSpecHandle& SpecHandle, const TArray<FHitResult>& HitResults)
{
	if (HitResults.IsEmpty()) return;

	if (!SpecHandle.IsValid()) return;

	APawn* const Owner = Cast<APawn>(GetAvatarActorFromActorInfo());
	if (!IsValid(Owner)) return;
	
	// 중복 제거용 
	TSet<AActor*> UniqueActors;

	// Set으로 중복되지 않는 액터배열 생성
	for (const FHitResult& HitResult : HitResults)
	{
		AActor* const HitActor = HitResult.GetActor();
		if (IsValid(HitActor))
		{
			UniqueActors.Add(HitActor);
		}
	}

	// 중복되지 않게 GameplayEffect 적용
	for (AActor* const HitActor : UniqueActors)
	{
		APawn* const HitPawn = Cast<APawn>(HitActor);
		if (!IsValid(HitPawn)) continue;

		// 팀 검사 
		const bool bIsValidTarget = URPGFunc::CheckTargetTeamAgent(Owner, HitPawn);
		if (!bIsValidTarget) continue;

		// 이펙트 적용
		const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = ApplyEffectsSpecHandleToTarget(HitPawn, SpecHandle);

		// 이펙트 적용 성공 시 히트 리액트 이벤트 전송
		const bool bWasSuccessfullyApplied = ActiveGameplayEffectHandle.WasSuccessfullyApplied();
		if (bWasSuccessfullyApplied)
		{
			FGameplayEventData EventData;
			EventData.Instigator = Owner;
			EventData.Target = HitPawn;

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitPawn, RPGGameplayTag::Character_Event_HitReact, EventData);
		}
	}
}