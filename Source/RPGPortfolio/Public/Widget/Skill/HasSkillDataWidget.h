// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "RPGStructTypes.h"
#include "HasSkillDataWidget.generated.h"

class UImage;
/**
 * UHasSkillDataWidget
 * 
 * 스킬 데이터를 가진 위젯
 */
UCLASS()
class RPGPORTFOLIO_API UHasSkillDataWidget : public URPGWidgetBase
{
	GENERATED_BODY()
public:
	//@ 스킬 데이터를 직접 업데이트
	UFUNCTION(BlueprintCallable)
	void UpdateSkillData(FPlayerAbilitySkillSet SkillSlotDataToSet);

	//@ 게임플레이 태그로 스킬 데이터 업데이트 (인덱스 기반)
	UFUNCTION(BlueprintCallable)
	void UpdateSkillDataFromGamePlayTag(int32 Index, FGameplayTag SkillTag);

	//@ 스킬 슬롯 데이터
	UPROPERTY(BlueprintReadWrite)
	FPlayerAbilitySkillSet SkillSlotData;
	//@ 스킬 아이콘 이미지
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* SkillIcon;

	//@ 슬롯 인덱스 (퀵슬롯 위치)
	UPROPERTY(BlueprintReadWrite)
	int32 SlotIndex;
protected:
	virtual void NativeOnInitialized() override;
};
