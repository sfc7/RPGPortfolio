// LJS


#include "Widget/Skill/SkillTreeQuickSlotContainer.h"

#include "GameplayTagContainer.h"
#include "Component/Player/PlayerSkillComponent.h"
#include "GameMode/GameManager/GASManager.h"
#include "Widget/Skill/HasSkillDataWidget.h"
#include "Components/Button.h"
#include "GameMode/GameManager/UIManager.h"

void USkillTreeQuickSlotContainer::NativeConstruct()
{
	Super::NativeConstruct();
	if (ApplyButton)
	{
		ApplyButton->OnClicked.AddDynamic(this, &USkillTreeQuickSlotContainer::OnApplyButtonClicked);
	}

	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &USkillTreeQuickSlotContainer::OnCancelButtonClicked);
	}
	
	UpdateQuickSlot();
}

void USkillTreeQuickSlotContainer::OnApplyButtonClicked()
{
	UPlayerSkillComponent* PlayerSkillComponent = GetWorld()->GetGameInstance()->GetSubsystem<UGASManager>()->GetPlayerSkillComponent();
	if (!IsValid(PlayerSkillComponent)) return;
	
	// 새로운 퀵슬롯 태그 배열 생성
	TArray<FGameplayTag> NewQuickSlots;
	NewQuickSlots.Add(QuickSlot_Z->SkillSlotData.SkillAbilityTag); 
	NewQuickSlots.Add(QuickSlot_X->SkillSlotData.SkillAbilityTag);  
	NewQuickSlots.Add(QuickSlot_C->SkillSlotData.SkillAbilityTag); 

	// 퀵슬롯 설정 적용
	PlayerSkillComponent->SetAllSkillQuickSlots(NewQuickSlots);

	GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->ToggleInputMode(GetWorld(), ERPGInputMode::GameMode);
}

void USkillTreeQuickSlotContainer::OnCancelButtonClicked()
{
	RemoveFromParent();
	GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->ToggleInputMode(GetWorld(), ERPGInputMode::GameMode);
}

void USkillTreeQuickSlotContainer::UpdateQuickSlot()
{
	UPlayerSkillComponent* PlayerSkillComponent = GetWorld()->GetGameInstance()->GetSubsystem<UGASManager>()->GetPlayerSkillComponent();
	if (!IsValid(PlayerSkillComponent)) return;

	// 퀵슬롯 인덱스 FGameplayTag 가져오기
	TArray<FGameplayTag> QuickSlotGameplayTags;
	if (PlayerSkillComponent)
	{
		for (int index = 0 ; index < PlayerSkillComponent->GetSkillSlotSize(); index++)
		{
			QuickSlotGameplayTags.Add(PlayerSkillComponent->GetSkillQuickSlotByIndex(index));
		}
	}

	TArray<UHasSkillDataWidget*> QuickSlotWidgets = { QuickSlot_Z, QuickSlot_X, QuickSlot_C };

	// 각 퀵슬롯 위젯 업데이트
	for (int32 Index = 0; Index < QuickSlotWidgets.Num(); Index++)
	{
		if (QuickSlotWidgets[Index])
		{
			FGameplayTag QuickSlotGameplayTag = PlayerSkillComponent->GetSkillQuickSlotByIndex(Index);
			FPlayerAbilitySkillSet QuickSlotSkillData;
			
			GetWorld()->GetGameInstance()->GetSubsystem<UGASManager>()->FindSkillByTag(QuickSlotGameplayTag, OUT QuickSlotSkillData);
			QuickSlotWidgets[Index]->UpdateSkillData(QuickSlotSkillData);
		}
	}
}
