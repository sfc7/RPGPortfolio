// LJS


#include "Widget/Skill/SkillTree.h"
#include "Widget/Skill/SkillContainer.h"
#include "Character/Player/PlayerCharacter_Fighter.h"

void USkillTree::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerCharacter_Fighter* PC = Cast<APlayerCharacter_Fighter>(GetOwningPlayerPawn());
	if (PC)
	{
		SkillContainer->SetSkillComponentRef(PC->GetPlayerSkillComponent());
	}
}
