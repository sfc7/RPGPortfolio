// LJS


#include "Widget/Skill/HasSkillDataWidget.h"

#include "Components/Image.h"
#include "GameMode/GameManager/GASManager.h"
#include "Component/Player/PlayerSkillComponent.h"


void UHasSkillDataWidget::UpdateSkillData(FPlayerAbilitySkillSet SkillSlotDataToSet)
{
	SkillSlotData = SkillSlotDataToSet;
	SkillIcon->SetBrushFromMaterial(SkillSlotData.AbilityIcon);
}

void UHasSkillDataWidget::UpdateSkillDataFromGamePlayTag(int32 Index, FGameplayTag SkillTag)
{
	if (Index == SlotIndex)
	{
		FPlayerAbilitySkillSet FoundSkillData;
		bool bSkillFound = GetWorld()->GetGameInstance()->GetSubsystem<UGASManager>()->FindSkillByTag(SkillTag, FoundSkillData);
    
		if (bSkillFound)
		{
			SkillSlotData = FoundSkillData;
			if (SkillIcon && SkillSlotData.AbilityIcon)
			{
				SkillIcon->SetBrushFromMaterial(SkillSlotData.AbilityIcon);
			}
		}
		else
		{
			SkillSlotData = FPlayerAbilitySkillSet();
			if (SkillIcon)
			{
				SkillIcon->SetBrushFromMaterial(nullptr);
			}
		}
	}
}

void UHasSkillDataWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}
