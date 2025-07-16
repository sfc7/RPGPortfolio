// LJS

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGStructTypes.h"
#include "GASManager.generated.h"

class UDataAsset_Player_SkillData;
class UPlayerSkillComponent;

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UGASManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UFUNCTION(BlueprintPure)
	TArray<FPlayerAbilitySkillSet> GetPlayerAbilitySkillSet() const;

	UFUNCTION(BlueprintPure)
	FString GetSkillDescriptionFromPlayerASC(FGameplayTag SkillTag);

	UFUNCTION(BlueprintPure)
	int32 GetSkillLevelFromPlayerASC(FGameplayTag SkillTag);
	
	UFUNCTION(BlueprintPure)
	bool FindSkillByTag(FGameplayTag SkillAbilityTag, FPlayerAbilitySkillSet& OutSkillData) const;

	UFUNCTION(BlueprintPure)
	bool CheckActiveSkillByTag(FGameplayTag SkillAbilityTag) const;

	UFUNCTION(BlueprintCallable)
	static bool ApplyGameplayEffectSpecHandleToTargetActor(AActor* Instigator,AActor* TargetActor,const FGameplayEffectSpecHandle& SpecHandle);

	UFUNCTION(BlueprintCallable)
	void OnSkillSlotDrop(UPlayerSkillComponent* FromSkillTree, UPlayerSkillComponent* ToSkillTreeQuickSlot) const;

	UFUNCTION(BlueprintCallable)
	UPlayerSkillComponent* GetPlayerSkillComponent();
	
protected:
	UPROPERTY(EditDefaultsOnly)
	UDataAsset_Player_SkillData* SkillDataAsset;
};
