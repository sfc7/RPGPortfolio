// LJS


#include "Widget/Skill/SkillImageMaster.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"

void USkillImageMaster::UpdateSlotData(FPlayerAbilitySkillSet UpdateSlotData)
{
	SkillSlotData = UpdateSlotData;
	SkillIcon->SetBrushFromMaterial(SkillSlotData.AbilityIcon);
}

void USkillImageMaster::SetSlotSizeBox(float Size)
{
	RPGSizeBox->SetWidthOverride(Size);
	RPGSizeBox->SetHeightOverride(Size);
}

void USkillImageMaster::SetSkillComponentRef(UPlayerSkillComponent* SkillComponentReftoSet)
{
	SkillComponentRef = SkillComponentReftoSet;	
}
