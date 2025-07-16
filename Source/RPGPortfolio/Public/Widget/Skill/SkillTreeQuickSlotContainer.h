// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "SkillTreeQuickSlotContainer.generated.h"

class UHasSkillDataWidget;
class UButton;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API USkillTreeQuickSlotContainer : public URPGWidgetBase
{
	GENERATED_BODY()
public:
	void UpdateQuickSlot();
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHasSkillDataWidget* QuickSlot_Z;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHasSkillDataWidget* QuickSlot_X;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHasSkillDataWidget* QuickSlot_C;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ApplyButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* CancelButton;
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnApplyButtonClicked();

	UFUNCTION()
	void OnCancelButtonClicked();
	
};
