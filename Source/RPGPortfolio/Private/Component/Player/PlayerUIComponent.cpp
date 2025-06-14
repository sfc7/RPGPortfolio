// LJS


#include "Component/Player/PlayerUIComponent.h"

void UPlayerUIComponent::UpdateSkillIconSlot(FGameplayTag SkillAbilityTag, TSoftObjectPtr<UMaterialInterface> SkillIconMaterial)
{
	OnSkillIconSlotUpdatedDelegate.Broadcast(SkillAbilityTag, SkillIconMaterial);
}
