// LJS


#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "Component/CombatComponentBase.h"

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
