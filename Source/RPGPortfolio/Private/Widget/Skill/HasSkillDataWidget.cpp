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
	// 인덱스가 일치해야됨
	if (Index == SlotIndex)
	{
		FPlayerAbilitySkillSet FoundSkillData;
		bool bSkillFound = GetWorld()->GetGameInstance()->GetSubsystem<UGASManager>()->FindSkillByTag(SkillTag, FoundSkillData);

		// 스킬을 찾은 경우 데이터 설정
		if (bSkillFound)
		{
			SkillSlotData = FoundSkillData;
			if (SkillIcon && SkillSlotData.AbilityIcon)
			{
				SkillIcon->SetBrushFromMaterial(SkillSlotData.AbilityIcon);
			}
		}
		// 스킬을 찾지 못한 경우 빈 데이터로 초기화
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
