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
#include "Component/Player/PlayerInventoryComponent.h"
#include "Character/RPGCharacterBase.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "DataAsset/DataAsset_PlayerAbilitySet.h"
#include "GameMode/GameManager/GASManager.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
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

	SkillImageSlot->SetSkillComponentRef(SkillComponentRef);
	SkillImageSlot->SkillIcon->SetBrushFromMaterial(SkillSlotData.AbilityIcon);
	SkillImageSlot->UpdateSlotData(SkillSlotData);
	SkillName->SetText(SkillSlotData.SkillName);
	SkillDescription->SetText(SkillSlotData.SkillDescription);
	SkillPrice->SetText(FText::AsNumber(SkillSlotData.SkillPrice));
	
	FString DetailDescription = GetWorld()->GetGameInstance()->GetSubsystem<UGASManager>()->GetSkillDescriptionFromPlayerASC(SkillSlotData.SkillAbilityTag);
	SkillDetailDescription->SetText(FText::FromString(DetailDescription));
	
	if (bool bHas = GetWorld()->GetGameInstance()->GetSubsystem<UGASManager>()->CheckActiveSkillByTag(SkillSlotData.SkillAbilityTag))
	{
		SkillDescriptionSwitcher->SetActiveWidgetIndex(1);
	}
}



bool USpellGlobe::CanPurchaseSkill()
{
	if (GetWorld()->GetGameInstance()->GetSubsystem<UGASManager>()->CheckActiveSkillByTag(SkillSlotData.SkillAbilityTag))
	{
		return false;
	}

	APlayerCharacterBase* Player = Cast<APlayerCharacterBase>(GetOwningPlayerPawn());
	if (!Player)
	{
		return false;
	}

	APlayerCharacter_Fighter* PlayerFighter = Cast<APlayerCharacter_Fighter>(Player);
	if (!PlayerFighter || !PlayerFighter->GetPlayerInventoryComponent())
	{
		return false;
	}

	int32 PlayerGold = PlayerFighter->GetPlayerInventoryComponent()->GetPlayerGold();
	return PlayerGold >= SkillSlotData.SkillPrice;
}

bool USpellGlobe::TryPurchaseSkill()
{
	if (!CanPurchaseSkill())
	{
		return false;
	}

	APlayerCharacterBase* Player = Cast<APlayerCharacterBase>(GetOwningPlayerPawn());
	if (!Player)
	{
		return false;
	}

	APlayerCharacter_Fighter* PlayerFighter = Cast<APlayerCharacter_Fighter>(Player);
	if (!PlayerFighter || !PlayerFighter->GetPlayerInventoryComponent())
	{
		return false;
	}

	URPGAbilitySystemComponent* ASC = Player->GetRPGAbilitySystemComponent();
	if (!ASC)
	{
		return false;
	}

	UPlayerInventoryComponent* PlayerInventory = PlayerFighter->GetPlayerInventoryComponent();


	FGameplayAbilitySpec AbilitySpec(SkillSlotData.AbilityToGrant); 
	AbilitySpec.SourceObject = ASC->GetAvatarActor();
	AbilitySpec.Level = 1; 
	AbilitySpec.GetDynamicSpecSourceTags().AddTag(SkillSlotData.SkillAbilityTag);
    
	FGameplayAbilitySpecHandle SpecHandle = ASC->GiveAbility(AbilitySpec);
    
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
	if (TryPurchaseSkill())
	{
		UpdateSkillData(SkillSlotData);
	}
}
