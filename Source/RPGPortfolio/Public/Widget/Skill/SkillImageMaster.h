// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "RPGStructTypes.h"
#include "SkillImageMaster.generated.h"

class UImage;
class USizeBox;
struct FPlayerAbilitySkillSet;
class UPlayerSkillComponent;
/**
 * USkillImageMaster
 * 
 * 스킬 아이콘 UI
 */
UCLASS()
class RPGPORTFOLIO_API USkillImageMaster : public URPGWidgetBase
{
	GENERATED_BODY()
	
public:
	//@ 스킬 아이콘
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* SkillIcon;

	//@ UI 전체 크기 박스
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USizeBox* RPGSizeBox;

	//@ 스킬 아이콘 업데이트
	UFUNCTION(BlueprintCallable)
	void UpdateSlotData(FPlayerAbilitySkillSet UpdateSlotData);

	//@ 스킬 UI 박스 사이즈 설정
	UFUNCTION(BlueprintCallable)
	void SetSlotSizeBox(float Size);

	//@ 스킬컴포넌트 참조 등록
	UFUNCTION(BlueprintCallable)
	void SetSkillComponentRef(UPlayerSkillComponent* SkillComponentReftoSet);
	
protected:
	//@ 스킬 AbiilitySet 정보
	UPROPERTY(BlueprintReadWrite)
	FPlayerAbilitySkillSet SkillSlotData;

	//@ 스킬컴포넌트 참조
	UPROPERTY(BlueprintReadWrite)
	UPlayerSkillComponent* SkillComponentRef;
};
