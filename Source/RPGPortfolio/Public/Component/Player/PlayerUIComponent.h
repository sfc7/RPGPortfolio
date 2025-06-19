// LJS

#pragma once

#include "CoreMinimal.h"
#include "Component/UIComponentBase.h"
#include "GameplayTagContainer.h"
#include "PlayerUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInitPlayerUIbyClassDelegate, EPlayerCharacterClass , PlayerCharacterClass);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillIconSlotUpdatedDelegate, FGameplayTag, SkillAbilityTag, TSoftObjectPtr<UMaterialInterface>, SkillIconMaterial);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSkillCooldownBeginDelegate, FGameplayTag, SkillAbilityTag, float, TotalCooldownTime, float, RemaningCooldownTime);
/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPORTFOLIO_API UPlayerUIComponent : public UUIComponentBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentMpChanged;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnInitPlayerUIbyClassDelegate OnInitPlayerUIbyClassDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnSkillIconSlotUpdatedDelegate OnSkillIconSlotUpdatedDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnSkillCooldownBeginDelegate OnSkillCooldownBeginDelegate;
	
	void UpdateSkillIconSlot(FGameplayTag SkillAbilityTag, TSoftObjectPtr<UMaterialInterface> SkillIconMaterial);
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameOver", meta = (AllowPrivateAccess = "true"))
	TSoftClassPtr<UUserWidget> GameOverUIWidgetClass;
};
	