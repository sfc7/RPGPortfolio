#pragma once


#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "RPGStructTypes.generated.h"

class URPGGameplayAbility;

USTRUCT(BlueprintType)
struct FPlayerAbilitySet
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<URPGGameplayAbility> AbilityToGrant;

	bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FPlayerWeaponData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FPlayerAbilitySet> DefaultWeaponAbilities;
	
};

USTRUCT(BlueprintType)
struct FWeaponDefaultData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float WeaponBaseDamage;
};

UENUM(BlueprintType)
enum class EWeaponAttackType : uint8
{
	Light,
	Heavy,
	Parrying,
	None
};

UENUM(BlueprintType)
enum class EPlayerCharacterClass : uint8
{
	Fighter,
	Magician,
	None
};

UENUM(BlueprintType)
enum class ERPGDecoratorComparisonOperator : uint8
{
	LessThan            UMETA(DisplayName = "Less Than"),
	LessThanOrEqualTo   UMETA(DisplayName = "Less Than or Equal To"),
	EqualTo             UMETA(DisplayName = "Equal To"),
	GreaterThanOrEqualTo UMETA(DisplayName = "Greater Than or Equal To"),
	GreaterThan         UMETA(DisplayName = "Greater Than"),
	None
};
