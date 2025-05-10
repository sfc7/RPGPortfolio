// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/RPGAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PlayerAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UPlayerAttributeSet : public URPGAttributeSet
{
	GENERATED_BODY()

public:
	UPlayerAttributeSet();

	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, CurrentLightAttackCombo);
	
protected:
	UPROPERTY(BlueprintReadOnly, Category="Attack", meta=(AllowPrivateAccess=true))
	FGameplayAttributeData CurrentLightAttackCombo;
	
};
