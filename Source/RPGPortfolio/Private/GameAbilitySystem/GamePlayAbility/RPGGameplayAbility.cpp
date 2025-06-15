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
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	check (TargetASC && _SpecHandle.IsValid());
	
	return GetRPGAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*_SpecHandle.Data,
		TargetASC
	);
}

void URPGGameplayAbility::AddGameplayTag(AActor* _Actor, FGameplayTag _GameplayTag)
{
	URPGAbilitySystemComponent* ASC = GetRPGAbilitySystemComponentFromActorInfo();

	if (!ASC->HasMatchingGameplayTag(_GameplayTag))
	{
		ASC->AddLooseGameplayTag(_GameplayTag);
	}
}

void URPGGameplayAbility::RemoveGameplayTag(AActor* _Actor, FGameplayTag _GameplayTag)
{
	URPGAbilitySystemComponent* ASC = GetRPGAbilitySystemComponentFromActorInfo();

	if (ASC->HasMatchingGameplayTag(_GameplayTag))
	{
		ASC->RemoveLooseGameplayTag(_GameplayTag);
	}
}

bool URPGGameplayAbility::HasMatchingGameplayTag(FGameplayTag _GameplayTag)	
{
	URPGAbilitySystemComponent* ASC = GetRPGAbilitySystemComponentFromActorInfo();

	return ASC->HasMatchingGameplayTag(_GameplayTag);
}

void URPGGameplayAbility::ApplyGameplayEffectgSpecHandleToHitResults(const FGameplayEffectSpecHandle& SpecHandle, const TArray<FHitResult>& HitResults)
{
	if (HitResults.IsEmpty())
	{
		return;
	}

	APawn* Owner = Cast<APawn>(GetAvatarActorFromActorInfo());
	
	TSet<AActor*> UniqueActors;

	for (const FHitResult& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			UniqueActors.Add(HitActor);
		}
	}

	for (AActor* HitActor : UniqueActors)
	{
		if (APawn* HitPawn = Cast<APawn>(HitActor))
		{
			if (URPGFunc::CheckTargetTeamAgent(Owner, HitPawn))
			{
				FActiveGameplayEffectHandle ActiveGameplayEffectHandle = ApplyEffectsSpecHandleToTarget(HitPawn, SpecHandle);

				if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
				{
					FGameplayEventData EventData;
					EventData.Instigator = Owner;
					EventData.Target = HitPawn;

					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitPawn, RPGGameplayTag::Character_Event_HitReact, EventData);
				}
			}
		}
	}
}