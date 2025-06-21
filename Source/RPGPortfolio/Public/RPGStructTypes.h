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
struct FPlayerAbilitySkillSet : public FPlayerAbilitySet
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UMaterialInterface> AbilityIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SkillAbilityTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SkillCooldown;
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
	Skill,
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

UENUM()
enum class ERPGCountDownActionInput : uint8
{
	Start,
	Cancel
};

UENUM()
enum class ERPGCountDownActionOutput : uint8
{
	Updated,
	Completed,
	Cancelled
};

UENUM()
enum class ERPGInputMode : uint8
{
	GameMode,
	UIMode,
};
