// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "SkillContainer.generated.h"

class UUniformGridPanel;
class UScrollBox;
class UPlayerSkillComponent;
class USpellGlobe;
/**
 * USkillContainer
 * 
 * 스킬 컨테이너 
 */
UCLASS()
class RPGPORTFOLIO_API USkillContainer : public URPGWidgetBase
{
	GENERATED_BODY()
public:
	USkillContainer(const FObjectInitializer& ObjectInitializer);

	//@ 스크롤 
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* ScrollBox;

	//@ 슬롯 그리드 패널
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUniformGridPanel* GridForSlots;

	//@ 행당 슬롯 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
	int32 SlotsPerRow;

	//@ 스킬 슬롯 새로고침, 그리드 패널 재구성 용
	UFUNCTION(BlueprintCallable)
	void RefreshSlots();

	//@ 슬롯 위젯 배열
	UPROPERTY(BlueprintReadWrite)
	TArray<USpellGlobe*> SlotWidgets;

	//@ 스킬 컴포넌트 참조 설정
	UFUNCTION(BlueprintCallable)
	void SetSkillComponentRef(UPlayerSkillComponent* SkillComponentRefToSet);

	//@ 스킬 컴포넌트 참조 설정 가져오기
	UFUNCTION(BlueprintCallable)
	UPlayerSkillComponent* GetSkillComponentRef(){ return SkillComponentRef; }
protected:
	virtual void NativeOnInitialized() override;

	//@ 스킬 글로브 위젯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> SkillGloberWidgetClass;
	
private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UPlayerSkillComponent* SkillComponentRef;
};
