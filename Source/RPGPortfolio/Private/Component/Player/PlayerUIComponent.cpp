// LJS


#include "Component/Player/PlayerUIComponent.h"

void UPlayerUIComponent::UpdateSkillIconSlot(FGameplayTag AbilityInputTag, TSoftObjectPtr<UMaterialInterface> SkillIconMaterial)
{
	OnSkillIconSlotUpdatedDelegate.Broadcast(AbilityInputTag, SkillIconMaterial);
}
