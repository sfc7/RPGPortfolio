// LJS


#include "GameAbilitySystem/RPGAbilitySystemComponent.h"

#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "DataAsset/SaveGame/RPGSaveGame.h"

void URPGAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& _InputTag)
{
	if (!_InputTag.IsValid()) return;
	
	// GetActivatableAbilities로 활성화 가능한 모든 어빌리티 검사
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// 동적 소스 태그가 입력 태그와 일치하지 않으면 스킵
		const bool bHasMatchingInputTag = AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(_InputTag);
		if (!bHasMatchingInputTag) continue;

		// 토글 타입 어빌리티 처리
		const bool bIsToggleType = _InputTag.MatchesTag(RPGGameplayTag::InputTag_Toggle);
		if (bIsToggleType)
		{
			const bool bIsCurrentlyActive = AbilitySpec.IsActive();
			if (bIsCurrentlyActive)
			{
				CancelAbilityHandle(AbilitySpec.Handle);
			}
			else
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
		else
		{
			// 어빌리티 활성화 실패 시 버퍼에 대한 처리
			const bool bActivated = TryActivateAbility(AbilitySpec.Handle);
			if (!bActivated)
			{
				float BufferCleartime = 0.5f;
				BufferedInputTag = _InputTag;
				GetWorld()->GetTimerManager().SetTimer(
					BufferTimerHandle, this,
					&URPGAbilitySystemComponent::ClearBuffer, BufferCleartime, false);
			}
		}
	}
}

void URPGAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& _InputTag)
{
	// 입력 태그 유효성 및 홀드 타입 검사
	const bool bIsValidInputTag = _InputTag.IsValid();
	const bool bIsHoldType = _InputTag.MatchesTag(RPGGameplayTag::InputTag_Hold);
	if (!bIsValidInputTag || !bIsHoldType) return;
	
	// 활성화된 홀드 타입 어빌리티 비활성화
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		const bool bHasMatchingInputTag = AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(_InputTag);
		const bool bIsCurrentlyActive = AbilitySpec.IsActive();
		
		if (bHasMatchingInputTag && bIsCurrentlyActive)
		{
			CancelAbilityHandle(AbilitySpec.Handle);
		}
	}
}

bool URPGAbilitySystemComponent::ActivateGamePlayAbilityByTag(FGameplayTag _ActivateAbilityTag)
{
	check(_ActivateAbilityTag.IsValid());

	// 해당 태그와 매칭되는 활성화 가능한 어빌리티들 찾기
	TArray<FGameplayAbilitySpec*> CanActiveAbilitySpecArray;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(_ActivateAbilityTag.GetSingleTagContainer(),CanActiveAbilitySpecArray);

	if (!CanActiveAbilitySpecArray.IsEmpty())
	{
		// 찾은 어빌리티들 중에서 랜덤하게 선택하여 활성화
		const int32 RandomIndex = FMath::RandRange(0, CanActiveAbilitySpecArray.Num()-1);
		FGameplayAbilitySpec* CanActiveAbilitySpec = CanActiveAbilitySpecArray[RandomIndex];
		if (CanActiveAbilitySpec && !CanActiveAbilitySpec->IsActive())
		{
			return TryActivateAbility(CanActiveAbilitySpec->Handle);
		}
	}

	return false;
}

FGameplayAbilitySpec* URPGAbilitySystemComponent::FindActiveAbilityByTag(FGameplayTag AbilityTag)
{
	if (!AbilityTag.IsValid()) return nullptr;

	// 활성화 가능한 모든 어빌리티에서 태그 검색
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		// 어빌리티의 에셋 태그들 검사
		for (const FGameplayTag& AssetTag : Spec.Ability->GetAssetTags())
		{
			if (AssetTag.MatchesTagExact(AbilityTag))
			{
				return &Spec;
			}
		}
	}
	
	return nullptr;
}

TArray<FGameplayAbilitySpec> URPGAbilitySystemComponent::GetAllSkillAbilities() const
{
	TArray<FGameplayAbilitySpec> SkillAbilities;

	// 활성화 가능한 모든 어빌리티에서 스킬 어빌리티 필터링
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		// 어빌리티의 에셋 태그들 검사
		for (const FGameplayTag& Tag : Spec.Ability->GetAssetTags())
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag("Player.Ability.Skill")))
			{
				SkillAbilities.Add(Spec);
				break;
			}
		}		
	}

	return SkillAbilities;
}

void URPGAbilitySystemComponent::SaveDynamicAbilitiesToSaveGame(URPGSaveGame* SaveGame)
{
	if (!IsValid(SaveGame)) return;

	TArray<FAbilitySpecSaveData> DynamicAbilities;

	// 모든 활성화 가능한 어빌리티에서 동적 어빌리티 필터링
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (IsDynamicAbility(Spec))
		{
			// 동적 어빌리티 데이터 생성 후 SaveData의 태그 배열에 추가
			FAbilitySpecSaveData SaveData;
			SaveData.AbilityClass = Spec.Ability->GetClass();
			SaveData.Level = Spec.Level;

			for (const FGameplayTag& Tag : Spec.GetDynamicSpecSourceTags())
			{
				SaveData.DynamicSpecSourceTags.Add(Tag);
			}
            
			DynamicAbilities.Add(SaveData);
		}
	}
    
	SaveGame->SetPlayerDynamicAbilities(DynamicAbilities);
}

void URPGAbilitySystemComponent::LoadDynamicAbilitiesFromSaveGame(URPGSaveGame* SaveGame)
{
	if (!IsValid(SaveGame)) return;

	// 기존 동적 어빌리티들 제거
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (IsDynamicAbility(Spec))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	// 동적 어빌리티들 제거
	for (const FGameplayAbilitySpecHandle& Handle : AbilitiesToRemove)
	{
		ClearAbility(Handle);
	}

	// 세이브 게임에서 동적 어빌리티 데이터 로드
	TArray<FAbilitySpecSaveData> DynamicAbilities = SaveGame->GetPlayerDynamicAbilities();
	for (const FAbilitySpecSaveData& SaveData : DynamicAbilities)
	{
		if (SaveData.AbilityClass.IsValid())
		{
			UClass* AbilityClass = SaveData.AbilityClass.LoadSynchronous();
			if (AbilityClass)
			{
				FGameplayAbilitySpec AbilitySpec(AbilityClass);
				AbilitySpec.SourceObject = GetAvatarActor();
				AbilitySpec.Level = SaveData.Level;

				// 동적 소스 태그들 복원
				for (const FGameplayTag& Tag : SaveData.DynamicSpecSourceTags)
				{
					AbilitySpec.GetDynamicSpecSourceTags().AddTag(Tag);
				}
                
				GiveAbility(AbilitySpec);
			}
		}
	}
}

void URPGAbilitySystemComponent::FlushInputBuffer()
{
	if (!BufferedInputTag.IsValid()) return;

	FGameplayTag TagToFlush = BufferedInputTag;
	ClearBuffer();                        
	OnAbilityInputPressed(TagToFlush);   
}

bool URPGAbilitySystemComponent::IsDynamicAbility(const FGameplayAbilitySpec& Spec) const
{
	// 동적 소스 태그가 존재하면 동적 어빌리티로 판단
	return Spec.GetDynamicSpecSourceTags().Num() > 0;
}

void URPGAbilitySystemComponent::ClearBuffer()
{
	BufferedInputTag = FGameplayTag::EmptyTag;
	GetWorld()->GetTimerManager().ClearTimer(BufferTimerHandle);
}
