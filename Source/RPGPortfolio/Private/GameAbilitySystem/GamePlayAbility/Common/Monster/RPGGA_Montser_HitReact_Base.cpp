// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Monster/RPGGA_Montser_HitReact_Base.h"
#include "Character/MonsterCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Kismet/KismetMathLibrary.h"

void URPGGA_Montser_HitReact_Base::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (bUseFaceAttacker)
	{
		FRotator FaceRotation = UKismetMathLibrary::FindLookAtRotation(GetMonsterCharacterFromActorInfo()->GetActorLocation(), TriggerEventData->Instigator->GetActorLocation());
		GetMonsterCharacterFromActorInfo()->SetActorRotation(FaceRotation);
	}
	
	if (bHasHitReactMontagesToPlay)
	{
		if (HitReactMontages.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, HitReactMontages.Num() - 1);

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
		FTimerHandle EndAbilityTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			EndAbilityTimerHandle,
			this,
			&URPGGA_Montser_HitReact_Base::OnEndAbilityCallback,
			0.2f,  
			false  
		);
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
