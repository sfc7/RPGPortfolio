// LJS


#include "Widget/Skill/SkillContainer.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "GameMode/GameManager/GASManager.h"
#include "Widget/Skill/SpellGlobe.h"

USkillContainer::USkillContainer(const FObjectInitializer& ObjectInitializer)
{
}

void USkillContainer::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void USkillContainer::RefreshSlots()
{
	// 기존 슬롯 정리
	GridForSlots->ClearChildren();
	SlotWidgets.Empty();
	
	TArray<FPlayerAbilitySkillSet> AllSkills = GetWorld()->GetGameInstance()->GetSubsystem<UGASManager>()->GetPlayerAbilitySkillSet();
	int32 TotalSkillCount = AllSkills.Num();

	// 각 스킬에 대해 글로브 생성해서 슬롯 배열에 추가후 그리드 패널에 추가하기
	for (int32 Index = 0; Index < TotalSkillCount; Index++)
	{	
		USpellGlobe* SpellGlobe = CreateWidget<USpellGlobe>(GetWorld()->GetFirstPlayerController(), SkillGloberWidgetClass);
		SpellGlobe->SetSkillComponentRef(SkillComponentRef);
		SpellGlobe->UpdateSkillData(AllSkills[Index]);
		
		SlotWidgets.Add(SpellGlobe);

		int32 Row = Index / SlotsPerRow;
		int32 LastColumn = Index % SlotsPerRow;
		
		UUniformGridSlot* CreateUniformGridSlot = GridForSlots->AddChildToUniformGrid(SpellGlobe, Row , LastColumn);
		CreateUniformGridSlot->SetHorizontalAlignment(HAlign_Center);
		CreateUniformGridSlot->SetVerticalAlignment(VAlign_Center);
	}
}

void USkillContainer::SetSkillComponentRef(UPlayerSkillComponent* SkillComponentRefToSet)
{	
	SkillComponentRef = SkillComponentRefToSet;
	
	RefreshSlots();
}
