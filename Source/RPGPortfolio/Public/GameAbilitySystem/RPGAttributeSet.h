// LJS

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RPGAttributeSet.generated.h"

//@ 세이브용 Attribute 구조체 (리플렉션이 지원되지 않아 만듬)
USTRUCT(BlueprintType)
struct FAttributeSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	FString AttributeName;

	UPROPERTY()
	float Value;
};

class IUIInterface;

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * URPGAttributeSet
 * 
 * 모든 ASC를 사용하는 캐릭터용 GameplayAbility AttributeSet
 */
UCLASS()
class RPGPORTFOLIO_API URPGAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	URPGAttributeSet();

	//@ GameplayEffect 실행 후 처리 - Attribute 변경 시 UI 업데이트 및 상태 관리
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

	//@ 소유자에게 GameplayTag 추가
	UFUNCTION(BlueprintCallable)
		void AddGameplayTagToOwner(FGameplayTag AddTag);

	//@ 소유자에게 GameplayTag 제거
	UFUNCTION(BlueprintCallable)
		void RemoveGameplayTagFromOwner(FGameplayTag RemoveTag);

	//@ 특정 대상에게 GameplayTag 추가
	UFUNCTION(BlueprintCallable)
		static void AddGameplayTagToActor(AActor* TargetActor, FGameplayTag AddTag);

	//@ 특정 대상 GameplayTag 제거
	UFUNCTION(BlueprintCallable)
		static void RemoveGameplayTagFromActor(AActor* TargetActor, FGameplayTag RemoveTag);

	//@ 모든 Attribute를 맵으로 저장(수치를 float으로)
	UFUNCTION(BlueprintCallable)
		TMap<FGameplayAttribute, float> SaveAllAttributes();

	//@ 맵에서 모든 Attribute를 로드
	UFUNCTION(BlueprintCallable)
		void LoadAllAttributes(TMap<FGameplayAttribute, float> AttributeMap);

	//@ 모든 Attribute를 세이브 데이터로 저장
	UFUNCTION(BlueprintCallable)
	TArray<FAttributeSaveData> SaveAllAttributesToSaveData();

	//@ 모든 Attribute를 세이브 데이터로 로드
	UFUNCTION(BlueprintCallable)
	void LoadAllAttributesFromSaveData(TArray<FAttributeSaveData> AttributeSaveData);

	//@ 장비 능력치 적용
	UFUNCTION(BlueprintCallable)
	void ApplyEquipmentStats(float AddMaxHp, float AddMaxMp, float AddAttackRate, float AddDefense);

	//@ 장비 능력치 적용 제거
	UFUNCTION(BlueprintCallable)
	void RemoveEquipmentStats(float RemoveMaxHp, float RemoveMaxMp, float RemoveAttackRate, float RemoveDefense);
private:
	//@ UI 인터페이스 약한 참조 (UI 업데이트용)
	TWeakInterfacePtr<IUIInterface> UIInterface;
};
