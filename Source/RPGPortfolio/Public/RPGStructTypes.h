#pragma once


#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "DataAsset/Item/DataAsset_RPGItemData_Equipment.h"

#include "RPGStructTypes.generated.h"

class URPGGameplayAbility;
class UDataAsset_RPGItemData;        
class UInventoryComponent;    
class UMaterialInterface;
class UPlayerEquipmentComponent;

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
	UMaterialInterface* AbilityIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SkillAbilityTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText SkillName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText SkillDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 SkillPrice;

};

USTRUCT(BlueprintType)
struct FWeaponDefaultData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float WeaponAttackRate;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float WeaponBaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName EquipSocketName;
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Melee,
	Body,
	Range,
	None
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

UENUM(BlueprintType)
enum class ERPGCountDownActionInput : uint8
{
	Start,
	Cancel
};

UENUM(BlueprintType)
enum class ERPGCountDownActionOutput : uint8
{
	Updated,
	Completed,
	Cancelled
};

UENUM(BlueprintType)
enum class ERPGInputMode : uint8
{
	GameMode,
	UIMode,
};

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SlotIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UDataAsset_RPGItemData> ItemDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInventoryComponent> InventoryRef;
};

UENUM(BlueprintType)
enum class EInteractType : uint8
{
	NPC,
	None,
};

UENUM(BlueprintType)
enum class ENPCType : uint8
{
	Conversational,
	Storage,
	Store,
	Quest,
	None,
};

USTRUCT(BlueprintType)
struct FInteractionData {
	GENERATED_USTRUCT_BODY()

	FInteractionData() : 
		CurrentInteractable(nullptr), 
		LastInteractionCheckTime(0.0f) 
	{};

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};