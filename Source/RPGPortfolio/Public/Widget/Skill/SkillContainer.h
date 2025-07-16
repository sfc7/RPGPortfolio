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
 * 
 */
UCLASS()
class RPGPORTFOLIO_API USkillContainer : public URPGWidgetBase
{
	GENERATED_BODY()
public:
	USkillContainer(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* ScrollBox;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUniformGridPanel* GridForSlots;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
	int32 SlotsPerRow;

	UFUNCTION(BlueprintCallable)
	void RefreshSlots();

	UPROPERTY(BlueprintReadWrite)
	TArray<USpellGlobe*> SlotWidgets;
	
	UFUNCTION(BlueprintCallable)
	void SetSkillComponentRef(UPlayerSkillComponent* SkillComponentRefToSet);

	UFUNCTION(BlueprintCallable)
	UPlayerSkillComponent* GetSkillComponentRef(){ return SkillComponentRef; }
protected:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> SkillGloberWidgetClass;
	
private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UPlayerSkillComponent* SkillComponentRef;
};
