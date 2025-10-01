// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "SkillTree.generated.h"

class USkillContainer;
class UHasSkillDataWidget;
class USkillTreeQuickSlotContainer;
/**
 * USkillTree
 * 
 * 스킬 UI 창
 */
UCLASS()
class RPGPORTFOLIO_API USkillTree : public URPGWidgetBase
{
	GENERATED_BODY()
public:
	//@ 스킬 UI 컨테이너
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USkillContainer* SkillContainer;

	//@ 퀵슬롯 등록창 컨테이너
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USkillTreeQuickSlotContainer* SkillTreeQuickSlotContainer;

	
protected:
	virtual void NativeConstruct() override;
};
