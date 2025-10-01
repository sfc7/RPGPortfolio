// LJS


#include "Widget/Skill/SpellGlobe.h"

#include "GameplayAbilitySpec.h"
#include "GameplayAbilitySpecHandle.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "Character/Player/PlayerCharacter_Fighter.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/SizeBox.h"
#include "Widget/ItemInfomation.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "GameMode/GameManager/GASManager.h"
#include "Materials/MaterialInterface.h"
#include "Component/InventoryComponent.h"
#include "Character/RPGCharacterBase.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "DataAsset/DataAsset_PlayerAbilitySet.h"
#include "GameMode/GameManager/GASManager.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "Component/Player/PlayerInventoryComponent.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "Widget/Skill/SkillImageMaster.h"

void USpellGlobe::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (SkillBuyButton)
	{
		SkillBuyButton->OnClicked.AddDynamic(this, &USpellGlobe::OnSkillBuyButtonClicked);
	}
	
}

void USpellGlobe::SetSkillComponentRef(UPlayerSkillComponent* SkillComponentRefToSet)
{
	SkillComponentRef = SkillComponentRefToSet;
}

void USpellGlobe::UpdateSkillData(FPlayerAbilitySkillSet UpdateSkillData)
{
	SkillSlotData = UpdateSkillData;

	//@ UI용 스킬 데이터 업데이트
	SkillImageSlot->SetSkillComponentRef(SkillComponentRef);
	SkillImageSlot->SkillIcon->SetBrushFromMaterial(SkillSlotData.AbilityIcon);
	SkillImageSlot->UpdateSlotData(SkillSlotData);
	SkillName->SetText(SkillSlotData.SkillName);
	SkillDescription->SetText(SkillSlotData.SkillDescription);
	SkillPrice->SetText(FText::AsNumber(SkillSlotData.SkillPrice));
	
	FString DetailDescription = GetWorld()->GetGameInstance()->GetSubsystem<UGASManager>()->GetSkillDescriptionFromPlayerASC(SkillSlotData.SkillAbilityTag);
	SkillDetailDescription->SetText(FText::FromString(DetailDescription));

	// 상세 설명 가져오기
	if (GetWorld()->GetGameInstance()->GetSubsystem<UGASManager>()->CheckActiveSkillByTag(SkillSlotData.SkillAbilityTag))
	{
		SkillDescriptionSwitcher->SetActiveWidgetIndex(1);
	}
}



bool USpellGlobe::CanPurchaseSkill()
{
	const UGASManager* GASManager = GetWorld()->GetGameInstance()->GetSubsystem<UGASManager>();
	if (!IsValid(GASManager)) return false;
	
	const bool bHasSkill = GASManager->CheckActiveSkillByTag(SkillSlotData.SkillAbilityTag);
	if (bHasSkill) return false;
	
	const APlayerCharacterBase* Player = Cast<APlayerCharacterBase>(GetOwningPlayerPawn());
	if (!IsValid(Player)) return false;

	const APlayerCharacter_Fighter* PlayerFighter = Cast<APlayerCharacter_Fighter>(Player);
	if (!IsValid(PlayerFighter) || !IsValid(PlayerFighter->GetPlayerInventoryComponent())) return false;
	
	// 골드 확인
	const int32 PlayerGold = PlayerFighter->GetPlayerInventoryComponent()->GetPlayerGold();
	return PlayerGold >= SkillSlotData.SkillPrice;
}

bool USpellGlobe::TryPurchaseSkill()
{
	if (!CanPurchaseSkill()) return false;
	
	const APlayerCharacterBase* Player = Cast<APlayerCharacterBase>(GetOwningPlayerPawn());
	if (!IsValid(Player)) return false;

	const APlayerCharacter_Fighter* PlayerFighter = Cast<APlayerCharacter_Fighter>(Player);
	if (!IsValid(PlayerFighter) || !IsValid(PlayerFighter->GetPlayerInventoryComponent())) return false;

	URPGAbilitySystemComponent* ASC = Player->GetRPGAbilitySystemComponent();
	if (!IsValid(ASC)) return false;
	
	UInventoryComponent* PlayerInventory = PlayerFighter->GetPlayerInventoryComponent();
	if (!IsValid(PlayerInventory)) return false;

	// 어빌리티 스펙 생성
	FGameplayAbilitySpec AbilitySpec(SkillSlotData.AbilityToGrant); 
	AbilitySpec.SourceObject = ASC->GetAvatarActor();
	AbilitySpec.Level = 1; 
	AbilitySpec.GetDynamicSpecSourceTags().AddTag(SkillSlotData.SkillAbilityTag);
    
	FGameplayAbilitySpecHandle SpecHandle = ASC->GiveAbility(AbilitySpec);

	// 어빌리티 부여 성공 시 골드 차감
	if (SpecHandle.IsValid())
	{
		PlayerInventory->SetGold(-SkillSlotData.SkillPrice);
		return true;
	}
	else
	{
		return false;
	}
}

void USpellGlobe::OnSkillBuyButtonClicked()
{
	// 구매 시도 후 성공 시 UI 업데이트
	if (TryPurchaseSkill())
	{
		UpdateSkillData(SkillSlotData);
	}
}
