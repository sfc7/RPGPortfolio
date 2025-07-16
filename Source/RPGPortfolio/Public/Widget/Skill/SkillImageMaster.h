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
 * 
 */
UCLASS()
class RPGPORTFOLIO_API USkillImageMaster : public URPGWidgetBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* SkillIcon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USizeBox* RPGSizeBox;

	UFUNCTION(BlueprintCallable)
	void UpdateSlotData(FPlayerAbilitySkillSet UpdateSlotData);
	
	UFUNCTION(BlueprintCallable)
	void SetSlotSizeBox(float Size);

	UFUNCTION(BlueprintCallable)
	void SetSkillComponentRef(UPlayerSkillComponent* SkillComponentReftoSet);
	
protected:
	UPROPERTY(BlueprintReadWrite)
	FPlayerAbilitySkillSet SkillSlotData;

	UPROPERTY(BlueprintReadWrite)
	UPlayerSkillComponent* SkillComponentRef;
};
