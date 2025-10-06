// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/RPGAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PlayerAttributeSet.generated.h"



/**
 * UPlayerAttributeSet
 * 
 * 플레이어용 GameplayAbility AttributeSet
 */
UCLASS()
class RPGPORTFOLIO_API UPlayerAttributeSet : public URPGAttributeSet
{
	GENERATED_BODY()

public:
	UPlayerAttributeSet();

	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, CurrentLightAttackCombo);
	
protected:
	//@ 기본 공격 콤보 관련 속성
	UPROPERTY(BlueprintReadOnly, Category="Attack", meta=(AllowPrivateAccess=true))
	FGameplayAttributeData CurrentLightAttackCombo;
	
};
