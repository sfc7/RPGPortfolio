// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "RPGStructTypes.h"
#include "GameplayTagContainer.h"
#include "SpellGlobe.generated.h"

class UButton;
class UImage;
class UTextBlock;
class UPlayerSkillComponent;
class UWidgetSwitcher;
class USkillImageMaster;

/**
 * USpellGlobe
 * 
 * 스킬 UI의 단위
 */
UCLASS()
class RPGPORTFOLIO_API USpellGlobe : public URPGWidgetBase
{
	GENERATED_BODY()
public:
	//@ 스킬 컴포넌트 참조 설정
	UFUNCTION(BlueprintCallable)
	void SetSkillComponentRef(UPlayerSkillComponent* SkillComponentRefToSet);

	UFUNCTION(BlueprintCallable)
	UPlayerSkillComponent* GetSkillComponentRef(){ return SkillComponentRef; }

	//@ 스킬 데이터 업데이트 (UI 표시)
	UFUNCTION(BlueprintCallable)
	void UpdateSkillData(FPlayerAbilitySkillSet UpdateSlotData);
	
protected:
	virtual void NativeOnInitialized() override;

	//@ 스킬 슬롯 데이터
	UPROPERTY(BlueprintReadWrite)
	FPlayerAbilitySkillSet SkillSlotData;

	//@ 스킬 이미지 슬롯
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USkillImageMaster* SkillImageSlot;

	//@ 스킬 이름 텍스트
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SkillName;

	//@ 스킬 설명 텍스트
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SkillDescription;	

	//@ 스킬 상세 설명 텍스트
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SkillDetailDescription;

	//@ 스킬 가격 텍스트
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SkillPrice;

	//@ 스킬 구매 버튼
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* SkillBuyButton;

	//@ 스킬 설명 스위처 (구매 전/후 UI 전환)
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWidgetSwitcher* SkillDescriptionSwitcher;

	//@ 스킬 컴포넌트 참조
	UPROPERTY(BlueprintReadWrite)
	UPlayerSkillComponent* SkillComponentRef;

	//@ 스킬 구매 가능 여부 확인
	UFUNCTION(BlueprintCallable)
	bool CanPurchaseSkill();

	//@ 스킬 구매 시도
	UFUNCTION(BlueprintCallable)
	bool TryPurchaseSkill();

	//@ 스킬 구매 버튼 클릭 이벤트 핸들러
	UFUNCTION()
	void OnSkillBuyButtonClicked();

};
