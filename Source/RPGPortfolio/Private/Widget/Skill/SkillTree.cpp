// LJS


#include "Widget/Skill/SkillTree.h"
#include "Widget/Skill/SkillContainer.h"
#include "Character/Player/PlayerCharacter_Fighter.h"

void USkillTree::NativeConstruct()
{
	Super::NativeConstruct();

	//@ 스킬 UI 컨테이너에 스킬컴포넌트 참조 등록
	APlayerCharacter_Fighter* PC = Cast<APlayerCharacter_Fighter>(GetOwningPlayerPawn());
	if (IsValid(PC))
	{
		SkillContainer->SetSkillComponentRef(PC->GetPlayerSkillComponent());
	}
}
