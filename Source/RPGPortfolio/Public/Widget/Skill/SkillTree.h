// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "SkillTree.generated.h"

class USkillContainer;
class UHasSkillDataWidget;
class USkillTreeQuickSlotContainer;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API USkillTree : public URPGWidgetBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USkillContainer* SkillContainer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USkillTreeQuickSlotContainer* SkillTreeQuickSlotContainer;

	
protected:
	virtual void NativeConstruct() override;
};
