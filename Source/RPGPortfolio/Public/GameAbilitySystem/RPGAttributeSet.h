// LJS

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RPGAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	URPGAttributeSet();

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;
	
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, CurrentHp);
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxHp);
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, CurrentMp);
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxMp);
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, AttackRate);
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, Defense);
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, Damage);
	
	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData CurrentHp;

	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData MaxHp;

	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData CurrentMp;

	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData MaxMp;

	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData AttackRate;

	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData Defense;

	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData Damage;

	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
	static void AddGameplayTagToActor(AActor* _Actor, FGameplayTag TagToAdd);
};
