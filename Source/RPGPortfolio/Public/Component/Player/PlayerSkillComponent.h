// LJS

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "RPGStructTypes.h"
#include "PlayerSkillComponent.generated.h"

class URPGAbilitySystemComponent;
struct FPlayerAbilitySkillSet;
class URPGSaveGame;

//@ 스킬 아이콘 슬롯 업데이트 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillIconSlotUpdatedDelegate, int32, SlotIndex, FGameplayTag, SkillAbilityTag);


/**
 * UPlayerSkillComponent
 * 
 * 플레이어 스킬 시스템 관리 컴포넌트, 스킬 퀵슬롯 관리, 스킬과 입력 태그 매핑, 스킬 데이터 저장/로드
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPORTFOLIO_API UPlayerSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerSkillComponent();

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnSkillIconSlotUpdatedDelegate OnSkillIconSlotUpdatedDelegate;

	//@ 어빌리티 시스템 컴포넌트와 연동 및 초기 스킬 설정
	virtual void GiveAbilitySystemComponent(URPGAbilitySystemComponent* _ASC,int32 ApplyLevel = 1);

	//@ 인덱스로 스킬 퀵슬롯 태그 반환
	UFUNCTION(BlueprintPure)
	FGameplayTag GetSkillQuickSlotByIndex(int32 Index) const;

	//@ 모든 스킬 퀵슬롯 반환 (태그단위)
	UFUNCTION(BlueprintPure)
	TArray<FGameplayTag> GetSkillQuickSlots() const { return SkillQuickSlots; }

	//@ 스킬 슬롯 반환 
	UFUNCTION(BlueprintPure)
	int32 GetSkillSlotSize() const { return SkillSlotSize; }

	//@ 스킬 태그로 퀵슬롯 인덱스 찾기
	UFUNCTION(BlueprintPure)
	int32 FindQuickSlotIndexByTag(FGameplayTag SkillAbilityTag) const;

	//@ 슬롯 인덱스에 해당하는 입력 태그 반환
	FGameplayTag GetInputTagForSlot(int32 SlotIndex);

	//@ 스킬 퀵슬롯을 세이브 게임에 저장 (GamePlayTag 태그단위)
	UFUNCTION(BlueprintCallable)
	void SaveSkillQuickSlotsToSaveGame(URPGSaveGame* SaveGame);

	//@ 세이브 게임에서 스킬 퀵슬롯 로드 (GamePlayTag 태그단위)
	UFUNCTION(BlueprintCallable)
	void LoadSkillQuickSlotsFromSaveGame(URPGSaveGame* SaveGame);

public:
	//@ 모든 스킬 퀵슬롯 설정
	UFUNCTION(BlueprintCallable)
	void SetAllSkillQuickSlots(const TArray<FGameplayTag>& NewSkillQuickSlots);
	
	//@ 모든 스킬에서 입력 태그 제거
	void ClearAllInputTagsFromSkills(URPGAbilitySystemComponent* ASC);

	//@ 모든 스킬에 입력 태그 적용
	void ApplyInputTagsToSkills(URPGAbilitySystemComponent* ASC);
	
protected:
	virtual void BeginPlay() override;

	//@ 스킬 퀵슬롯 (GamePlayTag 태그단위)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Skills")
	TArray<FGameplayTag> SkillQuickSlots;

	//@ 모든 스킬 (GamePlayTag 태그단위)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Skills")
	TArray<FGameplayTag> SkillSlots;

	//@ 스킬 슬롯 최대 크기
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 SkillSlotSize = 3;

private:
	//@ 스킬 퀵슬롯 초기화
	void InitializeSkillQuickSlots();
};
