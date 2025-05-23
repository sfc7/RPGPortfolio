// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Monster/RPGGA_Montser_Death_Base.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/MonsterCharacter.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"

URPGGA_Montser_Death_Base::URPGGA_Montser_Death_Base()
{
}

void URPGGA_Montser_Death_Base::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
	if (DeathMontages.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, DeathMontages.Num() -1);
	
		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
		TEXT("Montser_Death"), DeathMontages[RandomIndex],1.0f,  NAME_None,
	true, 1.0f, false);

		PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Montser_Death_Base::OnEndAbilityCallback);
		PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Montser_Death_Base::OnEndAbilityCallback);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Montser_Death_Base::OnEndAbilityCallback);
		PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Montser_Death_Base::OnEndAbilityCallback);
		PlayMontageTask->ReadyForActivation();
		
		URPGAbilitySystemComponent* ASC = GetRPGAbilitySystemComponentFromActorInfo();
		if (ASC)
		{
			FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
			ASC->ExecuteGameplayCue(DeathSoudnGameplayTag, EffectContext);
		}
	}
}

void URPGGA_Montser_Death_Base::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	GetMonsterCharacterFromActorInfo()->MonsterDeath();
}

void URPGGA_Montser_Death_Base::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
