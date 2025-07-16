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
 * 
 */
UCLASS()
class RPGPORTFOLIO_API USpellGlobe : public URPGWidgetBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetSkillComponentRef(UPlayerSkillComponent* SkillComponentRefToSet);

	UFUNCTION(BlueprintCallable)
	UPlayerSkillComponent* GetSkillComponentRef(){ return SkillComponentRef; }

	UFUNCTION(BlueprintCallable)
	void UpdateSkillData(FPlayerAbilitySkillSet UpdateSlotData);
	
protected:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(BlueprintReadWrite)
	FPlayerAbilitySkillSet SkillSlotData;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USkillImageMaster* SkillImageSlot;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SkillName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SkillDescription;	

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SkillDetailDescription;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SkillPrice;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* SkillBuyButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWidgetSwitcher* SkillDescriptionSwitcher;
	
	UPROPERTY(BlueprintReadWrite)
	UPlayerSkillComponent* SkillComponentRef;

	UFUNCTION(BlueprintCallable)
	bool CanPurchaseSkill();

	UFUNCTION(BlueprintCallable)
	bool TryPurchaseSkill();
	
	UFUNCTION()
	void OnSkillBuyButtonClicked();

};
