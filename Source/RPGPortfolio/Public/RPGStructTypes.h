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

//@ 플레이어 Ability 구조체
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

//@ 플레이어 스킬 Ability 구조체
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

//@ 무기 기본 데이터 구조체
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

//@ 무기 타입 열거형
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Melee,
	Body,
	Range,
	None
};

//@ 공격 타입 열거형
UENUM(BlueprintType)
enum class EWeaponAttackType : uint8
{
	Light,
	Heavy,
	Parrying,
	Skill,
	None
};

//@ 플레이어 캐릭터 클래스 열거형
UENUM(BlueprintType)
enum class EPlayerCharacterClass : uint8
{
	Fighter,
	Magician,
	None
};

//@ BT 데코레이터용 비교 연산자 열거형
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

//@ 커스텀 카운트다운 함수 액션 입력 열거형
UENUM(BlueprintType)
enum class ERPGCountDownActionInput : uint8
{
	Start,
	Cancel
};

//@ 커스텀 카운트다운 함수 액션 출력 열거형
UENUM(BlueprintType)
enum class ERPGCountDownActionOutput : uint8
{
	Updated,
	Completed,
	Cancelled
};

//@ UI 입력 모드 열거형
UENUM(BlueprintType)
enum class ERPGInputMode : uint8
{
	GameMode,
	UIMode,
};

//@ 인벤토리 슬롯 구조체, 가장 기본적인 아이템 데이터 단위
USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID; // 아이템 고유 번호

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity; // 아이템 양

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SlotIndex; // 아이템 슬롯 번호

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UDataAsset_RPGItemData> ItemDataAsset; // 아이템정보 약참조

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInventoryComponent> InventoryRef; //아이템이 속한 레퍼런스
};

//@ 상호작용 타입 열거형
UENUM(BlueprintType)
enum class EInteractType : uint8
{
	NPC,
	None,
};

//@ NPC 타입 열거형
UENUM(BlueprintType)
enum class ENPCType : uint8
{
	Conversational,
	Storage,
	Store,
	Quest,
	None,
};

//@ 상호작용 데이터 구조체  
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