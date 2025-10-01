// LJS

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGStructTypes.h"
#include "GASManager.generated.h"

class UDataAsset_Player_SkillData;
class UPlayerSkillComponent;

/**
 * UGASManager
 * 
 * GAS 관리 서브시스템
 */
UCLASS()
class RPGPORTFOLIO_API UGASManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	//@ 플레이어 스킬 어빌리티 반환 
	UFUNCTION(BlueprintPure)
	TArray<FPlayerAbilitySkillSet> GetPlayerAbilitySkillSet() const;

	//@ 플레이어 ASC에서 스킬 설명 가져오기
	UFUNCTION(BlueprintPure)
	FString GetSkillDescriptionFromPlayerASC(FGameplayTag SkillTag);

	//@ 플레이어 ASC에서 스킬 레벨 가져오기
	UFUNCTION(BlueprintPure)
	int32 GetSkillLevelFromPlayerASC(FGameplayTag SkillTag);

	//@ FGameplayTag로 스킬 어빌리티 데이터 찾기
	UFUNCTION(BlueprintPure)
	bool FindSkillByTag(FGameplayTag SkillAbilityTag, FPlayerAbilitySkillSet& OutSkillData) const;

	//@ FGameplayTag로 활성화 된 스킬 어빌리티 확인
	UFUNCTION(BlueprintPure)
	bool CheckActiveSkillByTag(FGameplayTag SkillAbilityTag) const;

	//@ 타겟 액터에게 GameplayEffect 적용
	UFUNCTION(BlueprintCallable)
	static bool ApplyGameplayEffectSpecHandleToTargetActor(AActor* Instigator,AActor* TargetActor,const FGameplayEffectSpecHandle& SpecHandle);

	//@ 스킬 슬롯 드롭 처리, UI 용
	UFUNCTION(BlueprintCallable)
	void OnSkillSlotDrop(UPlayerSkillComponent* FromSkillTree, UPlayerSkillComponent* ToSkillTreeQuickSlot) const;

	//@ 플레이어 스킬 컴포넌트 가져오기 
	UFUNCTION(BlueprintCallable)
	UPlayerSkillComponent* GetPlayerSkillComponent();
	
protected:
	//@ 스킬 데이터 에셋, 스킬 설명같은 정적 데이터 
	UPROPERTY(EditDefaultsOnly)
	UDataAsset_Player_SkillData* SkillDataAsset;
};
