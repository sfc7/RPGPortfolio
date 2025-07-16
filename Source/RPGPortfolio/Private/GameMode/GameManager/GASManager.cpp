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
	if (!SkillTag.IsValid())
	{
		return FString();
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return FString();

	APlayerCharacterBase* PlayerCharacter = Cast<APlayerCharacterBase>(PC->GetPawn());
	if (!PlayerCharacter) return FString();

	URPGAbilitySystemComponent* ASC = PlayerCharacter->GetRPGAbilitySystemComponent();
	if (!ASC) return FString();

	FGameplayAbilitySpec* AbilitySpec = ASC->FindActiveAbilityByTag(SkillTag);
	if (!AbilitySpec || !AbilitySpec->Ability) return *FString::Printf(TEXT("스킬을 아직 배우지 않았습니다."));
	

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
	if (!SkillTag.IsValid())
	{
		return 0;
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return 0;

	APlayerCharacterBase* PlayerCharacter = Cast<APlayerCharacterBase>(PC->GetPawn());
	if (!PlayerCharacter) return 0;

	URPGAbilitySystemComponent* ASC = PlayerCharacter->GetRPGAbilitySystemComponent();
	if (!ASC) return 0;

	FGameplayAbilitySpec* AbilitySpec = ASC->FindActiveAbilityByTag(SkillTag);
	if (!AbilitySpec || !AbilitySpec->Ability) return 0;

	if (UPlayerGameplayAbility* PlayerAbility = Cast<UPlayerGameplayAbility>(AbilitySpec->Ability))
	{
		return PlayerAbility->GetAbilityLevel();
	}

	return 0;
}

bool UGASManager::FindSkillByTag(FGameplayTag SkillAbilityTag, FPlayerAbilitySkillSet& OutSkillData) const
{
	if (!SkillDataAsset || !SkillAbilityTag.IsValid())
	{
		return false;
	}

	TArray<FPlayerAbilitySkillSet> AllSkills = SkillDataAsset->GetPlayerAbilitySkillSet();
	
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
	if (!SkillAbilityTag.IsValid())
	{
		return false;
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return false;

	APlayerCharacterBase* PlayerCharacter = Cast<APlayerCharacterBase>(PC->GetPawn());
	if (!PlayerCharacter) return false;

	URPGAbilitySystemComponent* ASC = PlayerCharacter->GetRPGAbilitySystemComponent();
	if (!ASC) return false;

	FGameplayAbilitySpec* AbilitySpec = ASC->FindActiveAbilityByTag(SkillAbilityTag);
	if (!AbilitySpec || !AbilitySpec->Ability) return false;

	return true; 
}

bool UGASManager::ApplyGameplayEffectSpecHandleToTargetActor(AActor* Instigator, AActor* TargetActor,const FGameplayEffectSpecHandle& SpecHandle)
{
	URPGAbilitySystemComponent* InstigatorASC = CastChecked<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Instigator));
	URPGAbilitySystemComponent* TargetActorASC = CastChecked<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));

	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = InstigatorASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, TargetActorASC);

	return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
}

void UGASManager::OnSkillSlotDrop(UPlayerSkillComponent* FromSkillTree, UPlayerSkillComponent* ToSkillTreeQuickSlot) const
{
	
}

UPlayerSkillComponent* UGASManager::GetPlayerSkillComponent()
{
	APlayerCharacterBase* PC = Cast<APlayerCharacterBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (PC)
	{
		return PC->GetPlayerSkillComponent();
	}

	return nullptr;
}
