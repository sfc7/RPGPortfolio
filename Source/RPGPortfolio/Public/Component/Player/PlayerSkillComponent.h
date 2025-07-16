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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillIconSlotUpdatedDelegate, int32, SlotIndex, FGameplayTag, SkillAbilityTag);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPORTFOLIO_API UPlayerSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerSkillComponent();

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnSkillIconSlotUpdatedDelegate OnSkillIconSlotUpdatedDelegate;
	
	virtual void GiveAbilitySystemComponent(URPGAbilitySystemComponent* _ASC,int32 ApplyLevel = 1);

	UFUNCTION(BlueprintPure)
	FGameplayTag GetSkillQuickSlotByIndex(int32 Index) const;
    
	UFUNCTION(BlueprintPure)
	TArray<FGameplayTag> GetSkillQuickSlots() const { return SkillQuickSlots; }

	UFUNCTION(BlueprintPure)
	int32 GetSkillSlotSize() const { return SkillSlotSize; }
	
	UFUNCTION(BlueprintPure)
	int32 FindQuickSlotIndexByTag(FGameplayTag SkillAbilityTag) const;

	FGameplayTag GetInputTagForSlot(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void SaveSkillQuickSlotsToSaveGame(URPGSaveGame* SaveGame);

	UFUNCTION(BlueprintCallable)
	void LoadSkillQuickSlotsFromSaveGame(URPGSaveGame* SaveGame);

public:
	UFUNCTION(BlueprintCallable)
	void SetAllSkillQuickSlots(const TArray<FGameplayTag>& NewSkillQuickSlots);

	void ClearAllInputTagsFromSkills(URPGAbilitySystemComponent* ASC);
	void ApplyInputTagsToSkills(URPGAbilitySystemComponent* ASC);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Skills")
	TArray<FGameplayTag> SkillQuickSlots;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Skills")
	TArray<FGameplayTag> SkillSlots;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 SkillSlotSize = 3;

private:
	void InitializeSkillsForNewGame();

	void InitializeSkillQuickSlots();
};
