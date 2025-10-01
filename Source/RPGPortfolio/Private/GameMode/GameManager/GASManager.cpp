// LJS


#include "GameMode/GameManager/GASManager.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayAbilitySpec.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "DataAsset/DataAsset_PlayerAbilitySet.h"
#include "DataAsset/DataAsset_Player_SkillData.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/Skill/PlayerAttackSkillGameplayAbility.h"


void UGASManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SkillDataAsset = LoadObject<UDataAsset_Player_SkillData>(nullptr, TEXT("/Game/MyProject/Player/DataAsset/DA_Player_Skill.DA_Player_Skill"));
}


TArray<FPlayerAbilitySkillSet> UGASManager::GetPlayerAbilitySkillSet() const
{
	return SkillDataAsset->GetPlayerAbilitySkillSet();
}

FString UGASManager::GetSkillDescriptionFromPlayerASC(FGameplayTag SkillTag)
{
	if (!SkillTag.IsValid()) return FString();
	
	APlayerController* const PC = GetWorld()->GetFirstPlayerController();
	if (!IsValid(PC)) return FString();

	APlayerCharacterBase* const PlayerCharacter = Cast<APlayerCharacterBase>(PC->GetPawn());
	if (!IsValid(PlayerCharacter)) return FString();

	URPGAbilitySystemComponent* const ASC = PlayerCharacter->GetRPGAbilitySystemComponent();
	if (!IsValid(ASC)) return FString();

	// 활성 어빌리티 스펙 찾기
	FGameplayAbilitySpec* AbilitySpec = ASC->FindActiveAbilityByTag(SkillTag);
	if (!AbilitySpec || !AbilitySpec->Ability) return *FString::Printf(TEXT("스킬을 아직 배우지 않았습니다."));
	
	// 스킬 어빌리티 타입에 따른 설명 반환
	if (UPlayerAttackSkillGameplayAbility* SkillAbility = Cast<UPlayerAttackSkillGameplayAbility>(AbilitySpec->Ability))
	{
		return SkillAbility->GetSkillDescriptionForUI(); 
	}
	else if (UPlayerGameplayAbility* PlayerAbility = Cast<UPlayerGameplayAbility>(AbilitySpec->Ability))
	{
		return PlayerAbility->GetDescriptionForUI();
	}
	
	return FString();
}

int32 UGASManager::GetSkillLevelFromPlayerASC(FGameplayTag SkillTag)
{
	if (!SkillTag.IsValid()) return 0;
	
	APlayerController* const PC = GetWorld()->GetFirstPlayerController();
	if (!IsValid(PC)) return 0;

	APlayerCharacterBase* const PlayerCharacter = Cast<APlayerCharacterBase>(PC->GetPawn());
	if (!IsValid(PlayerCharacter)) return 0;

	URPGAbilitySystemComponent* const ASC = PlayerCharacter->GetRPGAbilitySystemComponent();
	if (!IsValid(ASC)) return 0;

	// 활성 어빌리티 스펙 찾기
	FGameplayAbilitySpec* AbilitySpec = ASC->FindActiveAbilityByTag(SkillTag);
	if (!AbilitySpec || !AbilitySpec->Ability) return 0;

	// 플레이어 어빌리티에서 레벨 가져오기
	UPlayerGameplayAbility* const PlayerAbility = Cast<UPlayerGameplayAbility>(AbilitySpec->Ability);
	if (IsValid(PlayerAbility))
	{
		return PlayerAbility->GetAbilityLevel();
	}

	return 0;
}

bool UGASManager::FindSkillByTag(FGameplayTag SkillAbilityTag, FPlayerAbilitySkillSet& OutSkillData) const
{
	if (!IsValid(SkillDataAsset) || !SkillAbilityTag.IsValid()) return false;
	
	TArray<FPlayerAbilitySkillSet> AllSkills = SkillDataAsset->GetPlayerAbilitySkillSet();

	// 태그와 일치하는 스킬 데이터 찾기
	for (const FPlayerAbilitySkillSet& SkillData : AllSkills)
	{
		if (SkillData.SkillAbilityTag == SkillAbilityTag)
		{
			OutSkillData = SkillData;
			return true;
		}
	}

	return false;
}

bool UGASManager::CheckActiveSkillByTag(FGameplayTag SkillAbilityTag) const
{
	if (!SkillAbilityTag.IsValid()) return false;
	
	APlayerController* const PC = GetWorld()->GetFirstPlayerController();
	if (!IsValid(PC)) return false;

	APlayerCharacterBase* const PlayerCharacter = Cast<APlayerCharacterBase>(PC->GetPawn());
	if (!IsValid(PlayerCharacter)) return false;

	URPGAbilitySystemComponent* const ASC = PlayerCharacter->GetRPGAbilitySystemComponent();
	if (!IsValid(ASC)) return false;

	// 활성 어빌리티 스펙 찾기
	FGameplayAbilitySpec* AbilitySpec = ASC->FindActiveAbilityByTag(SkillAbilityTag);
	if (!AbilitySpec || !AbilitySpec->Ability) return false;

	return true; 
}

bool UGASManager::ApplyGameplayEffectSpecHandleToTargetActor(AActor* Instigator, AActor* TargetActor,const FGameplayEffectSpecHandle& SpecHandle)
{
	if (!IsValid(Instigator) || !IsValid(TargetActor)) return false;

	if (!SpecHandle.IsValid()) return false;
	
	URPGAbilitySystemComponent* InstigatorASC = CastChecked<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Instigator));
	URPGAbilitySystemComponent* TargetActorASC = CastChecked<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));

	// 게임플레이 이펙트 스펙을 타겟에게 적용
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = InstigatorASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, TargetActorASC);

	return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
}

void UGASManager::OnSkillSlotDrop(UPlayerSkillComponent* FromSkillTree, UPlayerSkillComponent* ToSkillTreeQuickSlot) const
{
	
}

UPlayerSkillComponent* UGASManager::GetPlayerSkillComponent()
{
	APlayerCharacterBase* PC = Cast<APlayerCharacterBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (IsValid(PC))
	{
		return PC->GetPlayerSkillComponent();
	}

	return nullptr;
}
