// LJS


#include "Component/Player/PlayerSkillComponent.h"

#include "GameplayAbilitySpec.h"
#include "Character/RPGCharacterBase.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "DataAsset/DataAsset_PlayerAbilitySet.h"
#include "GameMode/GameManager/GASManager.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "DataAsset/SaveGame/RPGSaveGame.h"

UPlayerSkillComponent::UPlayerSkillComponent()
{
}

void UPlayerSkillComponent::BeginPlay()
{
	Super::BeginPlay();
	
}
void UPlayerSkillComponent::GiveAbilitySystemComponent(URPGAbilitySystemComponent* _ASC, int32 ApplyLevel)
{
	if (APlayerCharacterBase* OwnerPlayer = Cast<APlayerCharacterBase>(GetOwner()))
	{
		TSoftObjectPtr<UDataAsset_AbilitySetBase> CharacterData = OwnerPlayer->GetCharacterStartUpData();
		if (UDataAsset_PlayerAbilitySet* PlayerAbilitySet = Cast<UDataAsset_PlayerAbilitySet>(CharacterData.LoadSynchronous()))
		{
			TArray<FGameplayTag> InitialSkills = PlayerAbilitySet->GetInitialQuickSlotSkills();
			
			SkillQuickSlots.Empty();
			for (int32 i = 0; i < SkillSlotSize; i++)
			{
				SkillQuickSlots.AddUnique(InitialSkills[i]);
				OnSkillIconSlotUpdatedDelegate.Broadcast(i, InitialSkills[i]);
			}
		}
	}
	
	int32 Index = 0;
	
	for (FGameplayTag Ability : SkillQuickSlots)
	{
		if (!Ability.IsValid())
		{
			Index++;
			continue;
		}
		
		FPlayerAbilitySkillSet SkillSet;
		if (GetWorld()->GetGameInstance()->GetSubsystem<UGASManager>()->FindSkillByTag(Ability, SkillSet))
		{
			if (Index == 0) SkillSet.InputTag = RPGGameplayTag::InputTag_FirstSkill_Keyboard;
			else if (Index == 1) SkillSet.InputTag = RPGGameplayTag::InputTag_SecondSkill_Keyboard;
			else if (Index == 2) SkillSet.InputTag = RPGGameplayTag::InputTag_ThirdSkill_Keyboard;
			
			FGameplayAbilitySpec AbilitySpec(SkillSet.AbilityToGrant);
			AbilitySpec.SourceObject = _ASC->GetAvatarActor();
			AbilitySpec.Level = ApplyLevel;
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(SkillSet.InputTag);
			
			_ASC->GiveAbility(AbilitySpec);
		}

		Index++;
	}
}

FGameplayTag UPlayerSkillComponent::GetSkillQuickSlotByIndex(int32 Index) const
{
	if (SkillQuickSlots.IsValidIndex(Index))
	{
		return SkillQuickSlots[Index];
	}
	
	return FGameplayTag();
}


int32 UPlayerSkillComponent::FindQuickSlotIndexByTag(FGameplayTag SkillAbilityTag) const
{
	for (int32 i = 0; i < SkillQuickSlots.Num(); i++)
	{
		if (SkillQuickSlots[i] == SkillAbilityTag)
		{
			return i;
		}
	}
    
	return -1; 
}

FGameplayTag UPlayerSkillComponent::GetInputTagForSlot(int32 SlotIndex)
{
	switch (SlotIndex)
	{
	case 0: return RPGGameplayTag::InputTag_FirstSkill_Keyboard;
	case 1: return RPGGameplayTag::InputTag_SecondSkill_Keyboard;
	case 2: return RPGGameplayTag::InputTag_ThirdSkill_Keyboard;
	default: return FGameplayTag();
	}
}

void UPlayerSkillComponent::SaveSkillQuickSlotsToSaveGame(URPGSaveGame* SaveGame)
{
	if (!SaveGame) return;
    
	SaveGame->SetPlayerSkillQuickSlots(SkillQuickSlots);
}

void UPlayerSkillComponent::LoadSkillQuickSlotsFromSaveGame(URPGSaveGame* SaveGame)
{
	if (!SaveGame) return;
    
	TArray<FGameplayTag> LoadedQuickSlots = SaveGame->GetPlayerSkillQuickSlots();
	if (LoadedQuickSlots.Num() > 0)
	{
		SkillQuickSlots = LoadedQuickSlots;
        
		for (int32 i = 0; i < SkillQuickSlots.Num(); i++)
		{
			OnSkillIconSlotUpdatedDelegate.Broadcast(i, SkillQuickSlots[i]);
		}
	}
}

void UPlayerSkillComponent::SetAllSkillQuickSlots(const TArray<FGameplayTag>& NewSkillQuickSlots)
{
	if (NewSkillQuickSlots.Num() != SkillSlotSize)
	{
		return;
	}

	APlayerCharacterBase* OwnerPlayer = Cast<APlayerCharacterBase>(GetOwner());
	if (!OwnerPlayer) return;

	URPGAbilitySystemComponent* ASC = OwnerPlayer->GetRPGAbilitySystemComponent();
	if (!ASC) return;

	ClearAllInputTagsFromSkills(ASC);
	SkillQuickSlots = NewSkillQuickSlots;
	ApplyInputTagsToSkills(ASC);
    
	for (int32 i = 0; i < SkillQuickSlots.Num(); i++)
	{
		OnSkillIconSlotUpdatedDelegate.Broadcast(i, SkillQuickSlots[i]);
	}
}

void UPlayerSkillComponent::ClearAllInputTagsFromSkills(URPGAbilitySystemComponent* ASC)
{
	if (!ASC) return;
    
	for (int32 i = 0; i < SkillQuickSlots.Num(); i++)
	{
		if (SkillQuickSlots[i].IsValid())
		{
			FGameplayTag InputTag = GetInputTagForSlot(i);
			if (InputTag.IsValid())
			{
				FGameplayAbilitySpec* FoundSpec = ASC->FindActiveAbilityByTag(SkillQuickSlots[i]);
				if (FoundSpec)
				{
					FoundSpec->GetDynamicSpecSourceTags().RemoveTag(InputTag);
				}
			}
		}
	}
}

void UPlayerSkillComponent::ApplyInputTagsToSkills(URPGAbilitySystemComponent* ASC)
{
	if (!ASC) return;
    
	for (int32 i = 0; i < SkillQuickSlots.Num(); i++)
	{
		if (SkillQuickSlots[i].IsValid())
		{
			FGameplayTag InputTag = GetInputTagForSlot(i);
			if (InputTag.IsValid())
			{
				FGameplayAbilitySpec* FoundSpec = ASC->FindActiveAbilityByTag(SkillQuickSlots[i]);
				if (FoundSpec)
				{
					FoundSpec->GetDynamicSpecSourceTags().AddTag(InputTag);
				}
			}
		}
	}
}




void UPlayerSkillComponent::InitializeSkillQuickSlots()
{
	if (APlayerCharacterBase* OwnerPlayer = Cast<APlayerCharacterBase>(GetOwner()))
	{
		TSoftObjectPtr<UDataAsset_AbilitySetBase> CharacterData = OwnerPlayer->GetCharacterStartUpData();
		if (UDataAsset_PlayerAbilitySet* PlayerAbilitySet = Cast<UDataAsset_PlayerAbilitySet>(CharacterData.LoadSynchronous()))
		{
			TArray<FGameplayTag> InitialSkills = PlayerAbilitySet->GetInitialQuickSlotSkills();
		}
	}
	// SkillQuickSlots = GetWorld()->GetGameInstance()->GetSubsystem<UGASManager>()->GetInitialQuickSlotSkills();

	// 	AActor* AvatarActor = _ASC->GetAvatarActor();
	// 	if (AvatarActor)
	// 	{
	// 		UPlayerUIComponent* PlayerUIComp = AvatarActor->FindComponentByClass<UPlayerUIComponent>();
	// 		if (PlayerUIComp)
	// 		{
	// 			PlayerUIComp->OnSkillIconSlotUpdatedDelegate.Broadcast(AbilitySkillSet.SkillAbilityTag, AbilitySkillSet.AbilityIcon);
	// 		}
	// 	}
}

void UPlayerSkillComponent::InitializeSkillsForNewGame()
{
	// LearnedSkills = GetWorld()->GetGameInstance()->GetSubsystem<UGASManager>()->GetPlayerInitialAbilitySkillSet();
	//
	// TArray<FPlayerAbilitySkillSet> InitialSkills = GetWorld()->GetGameInstance()->GetSubsystem<UGASManager>()->GetPlayerInitialAbilitySkillSet();
	// for (int32 index = 0; index < SkillSlotSize; index++)
	// {
	// 	SkillSlots.AddUnique(InitialSkills[index].SkillAbilityTag); 
	// }
}
