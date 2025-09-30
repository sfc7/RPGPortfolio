// LJS

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/DataAsset_AbilitySetBase.h"
#include "RPGStructTypes.h"
#include "DataAsset_PlayerAbilitySet.generated.h"

/**
 * UDataAsset_PlayerAbilitySet
 * 
 * 플레이어 GameplayAbility용 DataAsset
 */
UCLASS()
class RPGPORTFOLIO_API UDataAsset_PlayerAbilitySet : public UDataAsset_AbilitySetBase
{
	GENERATED_BODY()
	
public:
	//@ ASC에 플레이어 Ability들 부여
	virtual void GiveAbilitySystemComponent(URPGAbilitySystemComponent* _ASC,int32 ApplyLevel = 1);

	//@ 플레이어 초기 AbilitySet
	UFUNCTION(BlueprintPure)
	TArray<FPlayerAbilitySet> GetPlayerInitialAbilitySet() const { return PlayerInitialAbilitySet; }
	
	//@ 초기 퀵슬롯 FGameplayTag들 반환
	UFUNCTION(BlueprintPure)
	TArray<FGameplayTag> GetInitialQuickSlotSkills() const { return InitialQuickSlotSkills; }
	
private:
	//@ 플레이어 GameplayAbility
	UPROPERTY(EditDefaultsOnly, Category = "GA")
	TArray<FPlayerAbilitySet> PlayerInitialAbilitySet;

	//@ 퀵슬롯은 FGameplayTag으로 관리
	UPROPERTY(EditDefaultsOnly, Category = "Initial QuickSlot")
	TArray<FGameplayTag> InitialQuickSlotSkills;
};
