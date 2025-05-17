// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/RPGAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PlayerAttributeSet.generated.h"



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
