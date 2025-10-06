// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Monster/RPGGA_Montser_HitReact_Base.h"
#include "Character/MonsterCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"

void URPGGA_Montser_HitReact_Base::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//@ 피격시 상대방을 보는 방향으로 할건지 플래그
	if (bUseFaceAttacker)
	{
		FRotator FaceRotation = UKismetMathLibrary::FindLookAtRotation(GetMonsterCharacterFromActorInfo()->GetActorLocation(), TriggerEventData->Instigator->GetActorLocation());
		GetMonsterCharacterFromActorInfo()->SetActorRotation(FaceRotation);
	}

	//@ 피격 Montage를 사용할건지 체크 플래그
	if (bHasHitReactMontagesToPlay)
	{
		if (HitReactMontages.Num() > 0)
		{
			const int32 RandomIndex = FMath::RandRange(0, HitReactMontages.Num() - 1);

			// 피격 애니메이션 재생 AbilityTask 생성
			UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
	TEXT("Montser_HitReact"), HitReactMontages[RandomIndex], 1.0f,  NAME_None,
	true, 1.0f, false);

			PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Montser_HitReact_Base::OnEndAbilityCallback);
			PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Montser_HitReact_Base::OnEndAbilityCallback);
			PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Montser_HitReact_Base::OnEndAbilityCallback);
			PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Montser_HitReact_Base::OnEndAbilityCallback);
			PlayMontageTask->ReadyForActivation();
		}
	}
	else
	{
		// 없으면 0.2초후 어빌리티 종료
		FTimerHandle EndAbilityTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			EndAbilityTimerHandle,
			this,
			&URPGGA_Montser_HitReact_Base::OnEndAbilityCallback,
			0.2f,  
			false  
		);
	}

	//@ 몬스터가 공격 받는 중인지 GameplayTag를 부여할 GameplayEffect
	if (IsBeingAttackedGameplayEffect)
	{
		FGameplayEffectContextHandle ContextHandle = GetRPGAbilitySystemComponentFromActorInfo()->MakeEffectContext();
		ContextHandle.AddSourceObject(this);
	
		FGameplayEffectSpecHandle SpecHandle = GetRPGAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
			IsBeingAttackedGameplayEffect, GetAbilityLevel(), ContextHandle);
	
		GetRPGAbilitySystemComponentFromActorInfo()->BP_ApplyGameplayEffectSpecToSelf(SpecHandle);
	}
}

void URPGGA_Montser_HitReact_Base::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URPGGA_Montser_HitReact_Base::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
