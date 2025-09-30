// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameMode/GameManager/QuestManager.h"
#include "RPGStructTypes.h"
#include "GameAbilitySystem/RPGAttributeSet.h"
#include "RPGSaveGame.generated.h"

//@ 어빌리티 스펙 저장 데이터, 동적 어빌리티 저장용
USTRUCT(BlueprintType)
struct FAbilitySpecSaveData
{
	GENERATED_BODY()

	//@ 어빌리티 클래스 소프트 레퍼런스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UGameplayAbility> AbilityClass;

	//@ 동적 소스 태그들 (입력 바인딩 정보)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGameplayTag> DynamicSpecSourceTags;

	//@ 어빌리티 레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level = 1;
};
/**
 * URPGSaveGame
 * 
 * 세이브 데이터 관리 클래스
 */
UCLASS()
class RPGPORTFOLIO_API URPGSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	URPGSaveGame();

	//@ 현재 활성화된 퀘스트 목록 반환
	UFUNCTION(BlueprintPure)
	TArray<FName> GetCurrentActiveQuests() const { return CurrentActiveQuests; }

	//@ 완료된 퀘스트 목록 반환
	UFUNCTION(BlueprintPure)
	TArray<FName> GetCompletedQuests() const { return CompletedQuests; }

	//@ 퀘스트 진행 상황 반환
	UFUNCTION(BlueprintPure)
	TMap<FName, FQuestSaveData> GetQuestProgress() const { return QuestProgress; }
	
	//@ 플레이어 인벤토리 슬롯 배열 반환
	UFUNCTION(BlueprintPure)
	TArray<FInventorySlot> GetPlayerInventorySlots() const { return PlayerInventorySlots; }

	//@ 플레이어 포션 슬롯 배열 반환
	UFUNCTION(BlueprintPure)
	TArray<FInventorySlot> GetPlayerPotionSlots() const { return PlayerPotionSlots; }

	//@ 플레이어 골드 반환
	UFUNCTION(BlueprintPure)
	int32 GetPlayerGold() const { return PlayerGold; }

	//@ 퀘스트 상세 정보 세이브
	UFUNCTION(BlueprintCallable)
	void SaveQuestDetails(ARPGQuestSystemActor* Quest);
	
	//@ 현재 활성 퀘스트 목록 설정
	UFUNCTION(BlueprintCallable)
	void SetCurrentActiveQuests(TArray<FName> CurrentActiveQuestsToSet);
	
	//@ 완료된 퀘스트 목록 설정
	UFUNCTION(BlueprintCallable)
	void SetCompletedQuests(TArray<FName> CompletedQuestsToSet);

	//@ 플레이어 인벤토리 슬롯 설정
	UFUNCTION(BlueprintCallable)
	void SetPlayerInventorySlots(TArray<FInventorySlot> PlayerInventorySlotsToSet);

	//@ 플레이어 포션 슬롯 설정
	UFUNCTION(BlueprintCallable)
	void SetPlayerPotionSlots(TArray<FInventorySlot> PlayerPotionSlotsToSet);

	//@ 플레이어 골드 설정
	UFUNCTION(BlueprintCallable)
	void SetPlayerGold(int32 Gold);

	//@ 저장된 AttributeSet (FString으로) 반환
	UFUNCTION(BlueprintPure)
	TArray<FAttributeSaveData> GetSavedAttributes() const { return SavedAttributes; }

	//@ AttributeSet (FString으로) 설정
	UFUNCTION(BlueprintCallable)
	void SetSavedAttributes(const TArray<FAttributeSaveData>& Attributes) { SavedAttributes = Attributes; }

	//@ 플레이어에 동적으로 등록된 어빌리티들 반환
	UFUNCTION(BlueprintPure)
	TArray<FAbilitySpecSaveData> GetPlayerDynamicAbilities() const { return PlayerDynamicAbilities; }

	//@ 플레이어에 동적으로 등록된 어빌리티들 설정
	UFUNCTION(BlueprintCallable)
	void SetPlayerDynamicAbilities(const TArray<FAbilitySpecSaveData>& Abilities) { PlayerDynamicAbilities = Abilities; }

	//@ 플레이어 스킬 퀵슬롯 데이터 반환
	UFUNCTION(BlueprintPure)
	TArray<FGameplayTag> GetPlayerSkillQuickSlots() const { return PlayerSkillQuickSlots; }

	//@ 플레이어 스킬 퀵슬롯 설정
	UFUNCTION(BlueprintCallable)
	void SetPlayerSkillQuickSlots(const TArray<FGameplayTag>& QuickSlots) { PlayerSkillQuickSlots = QuickSlots; }

	//@ 현재 활성화중인 퀘스트 
	UPROPERTY() 
	TArray<FName> CurrentActiveQuests;

	//@ 완료된 퀘스트 
	UPROPERTY() 
	TArray<FName> CompletedQuests;

	//@ 퀘스트 진행상황 FName으로 저장
	UPROPERTY() 
	TMap<FName, FQuestSaveData> QuestProgress;

	//@ 플레이어 인벤토리 슬롯 데이터
	UPROPERTY()
	TArray<FInventorySlot> PlayerInventorySlots;

	//@ 플레이어 포션 퀵슬롯 데이터
	UPROPERTY()
	TArray<FInventorySlot> PlayerPotionSlots;

	UPROPERTY()
	int32 PlayerGold;

	//@ AttributeSet을 FString으로 저장 (AttributeSet은 리플렉션 지원안함)
	UPROPERTY()
	TArray<FAttributeSaveData> SavedAttributes;

	//@ 동적 어빌리티 저장 데이터
	UPROPERTY()
	TArray<FAbilitySpecSaveData> PlayerDynamicAbilities;

	//@ 플레이어 스킬 퀵슬롯 저장 데이터
	UPROPERTY()
	TArray<FGameplayTag> PlayerSkillQuickSlots;

};
