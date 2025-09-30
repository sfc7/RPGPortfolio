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
	if (!IsValid(_ASC)) return;

	const APlayerCharacterBase* OwnerPlayer = Cast<APlayerCharacterBase>(GetOwner());
	if (!IsValid(OwnerPlayer)) return;

	// 캐릭터 데이터 가져오기
	const TSoftObjectPtr<UDataAsset_AbilitySetBase> CharacterData = OwnerPlayer->GetCharacterStartUpData();
	UDataAsset_PlayerAbilitySet* const PlayerAbilitySet = Cast<UDataAsset_PlayerAbilitySet>(CharacterData.LoadSynchronous());
	if (!IsValid(PlayerAbilitySet)) return;

	// 초기 스킬 목록 가져오기
	const TArray<FGameplayTag> InitialSkills = PlayerAbilitySet->GetInitialQuickSlotSkills();
	
	// 스킬 퀵슬롯 초기화
	SkillQuickSlots.Empty();
	for (int32 i = 0; i < SkillSlotSize; i++)
	{
		SkillQuickSlots.AddUnique(InitialSkills[i]);
		OnSkillIconSlotUpdatedDelegate.Broadcast(i, InitialSkills[i]);
	}
	
	int32 Index = 0;
	
	for (const FGameplayTag Ability : SkillQuickSlots)
	{
		// 유효하지 않은 어빌리티 태그 스킵
		if (!Ability.IsValid())
		{
			Index++;
			continue;
		}
		
		// GAS 매니저에서 스킬 데이터 검색
		FPlayerAbilitySkillSet SkillSet;
		UGASManager* const GASManager = GetWorld()->GetGameInstance()->GetSubsystem<UGASManager>();
		if (!IsValid(GASManager)) return;

		const bool bFoundSkill = GASManager->FindSkillByTag(Ability, SkillSet);
		if (!bFoundSkill)
		{
			Index++;
			continue;
		}

		// 슬롯 인덱스에 따른 입력 태그 설정
		if (Index == 0) SkillSet.InputTag = RPGGameplayTag::InputTag_FirstSkill_Keyboard;
		else if (Index == 1) SkillSet.InputTag = RPGGameplayTag::InputTag_SecondSkill_Keyboard;
		else if (Index == 2) SkillSet.InputTag = RPGGameplayTag::InputTag_ThirdSkill_Keyboard;
		
		// 어빌리티 스펙 생성 및 동적 태그 추가 후 ASC에 등록 
		FGameplayAbilitySpec AbilitySpec(SkillSet.AbilityToGrant);
		AbilitySpec.SourceObject = _ASC->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(SkillSet.InputTag);
		
		_ASC->GiveAbility(AbilitySpec);

		Index++;
	}
}

FGameplayTag UPlayerSkillComponent::GetSkillQuickSlotByIndex(int32 Index) const
{
	if (!SkillQuickSlots.IsValidIndex(Index)) return FGameplayTag();
	
	return SkillQuickSlots[Index];
}


int32 UPlayerSkillComponent::FindQuickSlotIndexByTag(FGameplayTag SkillAbilityTag) const
{
	if (!SkillAbilityTag.IsValid()) return -1;

	// 스킬 퀵슬롯에서 해당 태그 검색
	for (int32 i = 0; i < SkillQuickSlots.Num(); i++)
	{
		if (SkillQuickSlots[i] == SkillAbilityTag)
		{
			return i;
		}
	}
    
	// 찾지 못한 경우
	return -1; 
}

FGameplayTag UPlayerSkillComponent::GetInputTagForSlot(int32 SlotIndex)
{
	// 슬롯 인덱스에 따른 입력 태그 매핑
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
	if (!IsValid(SaveGame)) return;
    
	SaveGame->SetPlayerSkillQuickSlots(SkillQuickSlots);
}

void UPlayerSkillComponent::LoadSkillQuickSlotsFromSaveGame(URPGSaveGame* SaveGame)
{
	if (!IsValid(SaveGame)) return;
    
	TArray<FGameplayTag> LoadedQuickSlots = SaveGame->GetPlayerSkillQuickSlots();
	if (LoadedQuickSlots.Num() > 0)
	{
		// 스킬 퀵슬롯 설정
		SkillQuickSlots = LoadedQuickSlots;
        
		for (int32 i = 0; i < SkillQuickSlots.Num(); i++)
		{
			// UI 업데이트를 위한 델리게이트 브로드캐스트
			OnSkillIconSlotUpdatedDelegate.Broadcast(i, SkillQuickSlots[i]);
		}
	}
}

void UPlayerSkillComponent::SetAllSkillQuickSlots(const TArray<FGameplayTag>& NewSkillQuickSlots)
{
	if (NewSkillQuickSlots.Num() != SkillSlotSize) return;
	
	APlayerCharacterBase* OwnerPlayer = Cast<APlayerCharacterBase>(GetOwner());
	if (!IsValid(OwnerPlayer)) return;

	URPGAbilitySystemComponent* ASC = OwnerPlayer->GetRPGAbilitySystemComponent();
	if (!IsValid(ASC)) return;

	// 기존 ASC에 InputTag 제거 후 새로 적용
	ClearAllInputTagsFromSkills(ASC);
	SkillQuickSlots = NewSkillQuickSlots;
	ApplyInputTagsToSkills(ASC);
    
	for (int32 i = 0; i < SkillQuickSlots.Num(); i++)
	{
		// UI 업데이트를 위한 델리게이트 브로드캐스트
		OnSkillIconSlotUpdatedDelegate.Broadcast(i, SkillQuickSlots[i]);
	}
}

void UPlayerSkillComponent::ClearAllInputTagsFromSkills(URPGAbilitySystemComponent* ASC)
{
	if (!IsValid(ASC)) return;
    
	// 모든 스킬 퀵슬롯에서 입력 태그 제거
	for (int32 i = 0; i < SkillQuickSlots.Num(); i++)
	{
		// 유효하지 않은 스킬 태그 스킵
		if (!SkillQuickSlots[i].IsValid()) continue;

		// 슬롯에 해당하는 입력 태그 가져오기
		const FGameplayTag InputTag = GetInputTagForSlot(i);
		if (!InputTag.IsValid()) continue;

		// ASC에서 해당 스킬의 어빌리티 스펙 찾기
		FGameplayAbilitySpec* const FoundSpec = ASC->FindActiveAbilityByTag(SkillQuickSlots[i]);
		if (!FoundSpec) continue;

		// 동적 소스 태그에서 입력 태그 제거
		FoundSpec->GetDynamicSpecSourceTags().RemoveTag(InputTag);
	}
}

void UPlayerSkillComponent::ApplyInputTagsToSkills(URPGAbilitySystemComponent* ASC)
{
	if (!IsValid(ASC)) return;
    
	// 모든 스킬 퀵슬롯에 입력 태그 적용
	for (int32 i = 0; i < SkillQuickSlots.Num(); i++)
	{
		// 유효하지 않은 스킬 태그 스킵
		if (!SkillQuickSlots[i].IsValid()) continue;

		// 슬롯에 해당하는 입력 태그 가져오기
		const FGameplayTag InputTag = GetInputTagForSlot(i);
		if (!InputTag.IsValid()) continue;

		// ASC에서 해당 스킬의 어빌리티 스펙 찾기
		FGameplayAbilitySpec* const FoundSpec = ASC->FindActiveAbilityByTag(SkillQuickSlots[i]);
		if (!FoundSpec) continue;

		// 동적 소스 태그에 입력 태그 추가
		FoundSpec->GetDynamicSpecSourceTags().AddTag(InputTag);
	}
}

void UPlayerSkillComponent::InitializeSkillQuickSlots()
{
	const APlayerCharacterBase* OwnerPlayer = Cast<APlayerCharacterBase>(GetOwner());
	if (!IsValid(OwnerPlayer)) return;

	// 캐릭터 데이터 가져오기
	const TSoftObjectPtr<UDataAsset_AbilitySetBase> CharacterData = OwnerPlayer->GetCharacterStartUpData();
	UDataAsset_PlayerAbilitySet* const PlayerAbilitySet = Cast<UDataAsset_PlayerAbilitySet>(CharacterData.LoadSynchronous());
	if (!IsValid(PlayerAbilitySet)) return;

	// 초기 스킬 목록 가져오기
	const TArray<FGameplayTag> InitialSkills = PlayerAbilitySet->GetInitialQuickSlotSkills();
}
