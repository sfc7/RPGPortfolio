// LJS


#include "Animation/AnimInstanceBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"

bool UAnimInstanceBase::DoesOwnerHaveTag(FGameplayTag TagToCheck) const
{
	APawn* OwningPawn = TryGetPawnOwner();
	if (!OwningPawn) return false;

	URPGAbilitySystemComponent* ASC = Cast<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwningPawn));
	if (ASC) return false;

	if (ASC->HasMatchingGameplayTag(TagToCheck)) return true;

	return false;	
}
