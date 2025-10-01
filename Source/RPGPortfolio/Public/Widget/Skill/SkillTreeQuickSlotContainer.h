// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "SkillTreeQuickSlotContainer.generated.h"

class UHasSkillDataWidget;
class UButton;
/**
 * USkillTreeQuickSlotContainer
 * 
 * 스킬 퀵슬롯 컨테이너 
 */
UCLASS()
class RPGPORTFOLIO_API USkillTreeQuickSlotContainer : public URPGWidgetBase
{
	GENERATED_BODY()
public:
	void UpdateQuickSlot();

	//@ Z 키 퀵슬롯 위젯
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHasSkillDataWidget* QuickSlot_Z;

	//@ X 키 퀵슬롯 위젯
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHasSkillDataWidget* QuickSlot_X;

	//@ C 키 퀵슬롯 위젯
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHasSkillDataWidget* QuickSlot_C;

	//@ 퀵슬롯세팅을 적용할지 버튼
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ApplyButton;

	//@ 퀵슬롯세팅을 취소할지 버튼
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* CancelButton;
	
protected:
	virtual void NativeConstruct() override;

	//@ 적용 버튼 클릭 이벤트 핸들러
	UFUNCTION()
	void OnApplyButtonClicked();

	//@ 취소 버튼 클릭 이벤트 핸들러
	UFUNCTION()
	void OnCancelButtonClicked();
	
};
