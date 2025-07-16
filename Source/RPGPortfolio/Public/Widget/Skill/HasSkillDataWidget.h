// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "RPGStructTypes.h"
#include "HasSkillDataWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UHasSkillDataWidget : public URPGWidgetBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void UpdateSkillData(FPlayerAbilitySkillSet SkillSlotDataToSet);

	UFUNCTION(BlueprintCallable)
	void UpdateSkillDataFromGamePlayTag(int32 Index, FGameplayTag SkillTag);
	
	UPROPERTY(BlueprintReadWrite)
	FPlayerAbilitySkillSet SkillSlotData;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* SkillIcon;

	UPROPERTY(BlueprintReadWrite)
	int32 SlotIndex;
protected:
	virtual void NativeOnInitialized() override;
};
