// LJS


#include "GameAbilitySystem/RPGAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "Character/MonsterCharacter.h"
#include "Component/UIComponentBase.h"
#include "Component/Monster/MonsterUIComponent.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Component/Player/PlayerUIComponent.h"
#include "GameMode/GameManager/CombatManager.h"
#include "Interface/UIInterface.h"
#include "WorldStatic/DamageIndicator.h"


URPGAttributeSet::URPGAttributeSet()
{
	InitCurrentHp(1.f),
	InitMaxHp(1.f),
	InitCurrentMp(1.f),
	InitMaxMp(1.f),
	InitAttackRate(1.f),
	InitDefense(1.f);
	InitDamage(1.f);
	
}

void URPGAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	// UI 인터페이스 초기화
	if (!UIInterface.IsValid())
	{
		UIInterface = TWeakInterfacePtr<IUIInterface>(Data.Target.GetAvatarActor());
	}

	check(UIInterface.IsValid());

	UUIComponentBase* const UIComponent = UIInterface->GetUIComponent();
	check(UIComponent);
		
	// 현재 HP 변경 처리
	if (Data.EvaluatedData.Attribute == GetCurrentHpAttribute())
	{
		URPGAbilitySystemComponent* const ASC = CastChecked<URPGAbilitySystemComponent>(GetOwningAbilitySystemComponent());

		// 패링 실패 중 ParryingSuccessCheck가 있을 때 타격 받는 경우 기록용
		const bool bIsParryingCheck = ASC->HasMatchingGameplayTag(RPGGameplayTag::Player_Status_ParryingSuccessCheck);
		if (bIsParryingCheck)
		{
			UCombatManager* const CombatManager = GetWorld()->GetGameInstance()->GetSubsystem<UCombatManager>();
			if (IsValid(CombatManager))
			{
				CombatManager->RecordParryAttempt(false);
			}
		}
		
		const float NewCurrentHp = FMath::Clamp(GetCurrentHp(), 0.f, GetMaxHp());
		SetCurrentHp(NewCurrentHp);
		
		const float HpRatio = GetCurrentHp() / GetMaxHp();
		UIComponent->OnCurrentHpChanged.Broadcast(HpRatio);
	}

	// 현재 MP 변경 처리
	if (Data.EvaluatedData.Attribute == GetCurrentMpAttribute())	
	{
		const float NewCurrentMp = FMath::Clamp(GetCurrentMp(), 0.f, GetMaxMp());
		SetCurrentMp(NewCurrentMp);

		// MP 상태에 따른 게임플레이 태그 관리
		AActor* const TargetActor = Data.Target.GetAvatarActor();
		const bool bIsMpFull = (GetCurrentMp() == GetMaxMp());

		// 마나 리젠용
		if (!bIsMpFull)
		{
			AddGameplayTagToActor(TargetActor, RPGGameplayTag::Player_Status_AttributeSet_MpNotFull);
			RemoveGameplayTagFromActor(TargetActor, RPGGameplayTag::Player_Status_AttributeSet_MpFull);
		}
		else
		{
			AddGameplayTagToActor(TargetActor, RPGGameplayTag::Player_Status_AttributeSet_MpFull);
			RemoveGameplayTagFromActor(TargetActor, RPGGameplayTag::Player_Status_AttributeSet_MpNotFull);
		}
		
		// 플레이어 UI 업데이트
		UPlayerUIComponent* const PlayerUIComponent = UIInterface->GetPlayerUIComponent();
		if (IsValid(PlayerUIComponent))
		{
			const float MpRatio = GetCurrentMp() / GetMaxMp();
			PlayerUIComponent->OnCurrentMpChanged.Broadcast(MpRatio);
		}
	}

	// 데미지 처리
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// 데미지 계산 및 HP 적용
		const float PreCurrentHp = GetCurrentHp();
		const float CalcDamage = GetDamage();
		const float CalcCurrentHp = FMath::Clamp(PreCurrentHp - CalcDamage, 0.f, GetMaxHp());

		SetCurrentHp(CalcCurrentHp);
		
		// HP UI 업데이트
		const float HpRatio = GetCurrentHp() / GetMaxHp();
		UIComponent->OnCurrentHpChanged.Broadcast(HpRatio);
		
		// 사망 상태 확인 및 태그 추가
		const bool bIsDead = (GetCurrentHp() == 0.f);
		if (bIsDead)
		{
			AActor* const TargetActor = Data.Target.GetAvatarActor();
			AddGameplayTagToActor(TargetActor, RPGGameplayTag::Character_Status_Dead);
		}

		// 몬스터인 경우 데미지 인디케이터 표시
		UMonsterUIComponent* const MonsterUIComponent = UIInterface->GetMonsterUIComponent();
		if (IsValid(MonsterUIComponent))
		{
			AMonsterCharacter* const Monster = MonsterUIComponent->GetOwningPawn<AMonsterCharacter>();
			if (IsValid(Monster))
			{
				Monster->SpawnDamageIndicator(CalcDamage);
			}
		}
	}
}

void URPGAttributeSet::AddGameplayTagToOwner(FGameplayTag AddTag)
{
	if (!AddTag.IsValid()) return;

	URPGAbilitySystemComponent* const ASC = CastChecked<URPGAbilitySystemComponent>(GetOwningAbilitySystemComponent());

	// 이미 태그가 있는지 확인 후 추가
	const bool bAlreadyHasTag = ASC->HasMatchingGameplayTag(AddTag);
	if (!bAlreadyHasTag)
	{
		ASC->AddLooseGameplayTag(AddTag);
	}
}

void URPGAttributeSet::RemoveGameplayTagFromOwner(FGameplayTag RemoveTag)
{
	if (!RemoveTag.IsValid()) return;

	URPGAbilitySystemComponent* const ASC = CastChecked<URPGAbilitySystemComponent>(GetOwningAbilitySystemComponent());

	// 태그가 있는지 확인 후 제거
	const bool bHasTag = ASC->HasMatchingGameplayTag(RemoveTag);
	if (bHasTag)
	{
		ASC->RemoveLooseGameplayTag(RemoveTag);
	}
}

TMap<FGameplayAttribute, float> URPGAttributeSet::SaveAllAttributes()
{
	TMap<FGameplayAttribute, float> LocalMap;

	// 모든 어트리뷰트 가져오기
	TArray<FGameplayAttribute> LocalAttributeArray;
	UAbilitySystemComponent* const OwningASC = GetOwningAbilitySystemComponent();
	if (!IsValid(OwningASC)) return LocalMap;

	OwningASC->GetAllAttributes(LocalAttributeArray);


	if (LocalAttributeArray.IsEmpty()) return LocalMap;

	// 모든 어트리뷰트 값을 맵에 저장
	for (const FGameplayAttribute& Attribute : LocalAttributeArray)
	{
		bool bFoundAttribute = false;
		const float AttributeValue = OwningASC->GetGameplayAttributeValue(Attribute, bFoundAttribute);

		if (bFoundAttribute)
		{
			LocalMap.Add(Attribute, AttributeValue);				
		}
	}

	return LocalMap;
}


void URPGAttributeSet::LoadAllAttributes(TMap<FGameplayAttribute, float> AttributeMap)
{
	if (AttributeMap.IsEmpty()) return;

	UAbilitySystemComponent* const OwningASC = GetOwningAbilitySystemComponent();
	if (!IsValid(OwningASC)) return;

	// 모든 어트리뷰트 값 적용
	for (const auto& Attribute : AttributeMap)
	{
		OwningASC->ApplyModToAttribute(Attribute.Key, EGameplayModOp::Override, Attribute.Value);
	}
}

TArray<FAttributeSaveData> URPGAttributeSet::SaveAllAttributesToSaveData()
{
	TArray<FAttributeSaveData> SaveDataArray;
    
	// 모든 어트리뷰트 가져오기
	TArray<FGameplayAttribute> LocalAttributeArray;
	UAbilitySystemComponent* const OwningASC = GetOwningAbilitySystemComponent();
	if (!IsValid(OwningASC)) return SaveDataArray;

	OwningASC->GetAllAttributes(LocalAttributeArray);
    
	if (LocalAttributeArray.IsEmpty()) return SaveDataArray;

	// 모든 어트리뷰트를 세이브 데이터로 변환
	for (const FGameplayAttribute& Attribute : LocalAttributeArray)
	{
		bool bFoundAttribute = false;
		const float AttributeValue = OwningASC->GetGameplayAttributeValue(Attribute, bFoundAttribute);
        
		if (bFoundAttribute)
		{
			FAttributeSaveData SaveData;
			SaveData.AttributeName = Attribute.GetName();
			SaveData.Value = AttributeValue;
			SaveDataArray.Add(SaveData);
		}
	}
    
	return SaveDataArray;
}

void URPGAttributeSet::LoadAllAttributesFromSaveData(TArray<FAttributeSaveData> AttributeSaveData)
{
	if (AttributeSaveData.IsEmpty()) return;

	UAbilitySystemComponent* const OwningASC = GetOwningAbilitySystemComponent();
	if (!IsValid(OwningASC)) return;

	// 현재 어트리뷰트 맵 가져오기
	const TMap<FGameplayAttribute, float> AttributeMap = SaveAllAttributes(); 
    
	// 세이브 데이터의 각 어트리뷰트 로드
	for (const FAttributeSaveData& SaveData : AttributeSaveData)
	{
		// 해당 어트리뷰트 찾기
		for (const auto& Pair : AttributeMap)
		{
			const bool bAttributeNameMatches = (Pair.Key.GetName() == SaveData.AttributeName);
			if (bAttributeNameMatches)
			{
				OwningASC->ApplyModToAttribute(Pair.Key, EGameplayModOp::Override, SaveData.Value);
				break;
			}
		}
	}
	
	// UI 인터페이스 초기화
	if (!UIInterface.IsValid())
	{
		UAbilitySystemComponent* const ASC = GetOwningAbilitySystemComponent();
		if (IsValid(ASC))
		{
			AActor* const AvatarActor = ASC->GetAvatarActor();
			if (IsValid(AvatarActor))
			{
				UIInterface = TWeakInterfacePtr<IUIInterface>(AvatarActor);
			}
		}
	}
	
	// UI 업데이트
	if (!UIInterface.IsValid()) return;

	UUIComponentBase* const UIComponent = UIInterface->GetUIComponent();
	if (IsValid(UIComponent))
	{
		const float HpRatio = GetCurrentHp() / GetMaxHp();
		UIComponent->OnCurrentHpChanged.Broadcast(HpRatio);
	}
	
	UPlayerUIComponent* const PlayerUIComponent = UIInterface->GetPlayerUIComponent();
	if (IsValid(PlayerUIComponent))
	{
		const float MpRatio = GetCurrentMp() / GetMaxMp();
		PlayerUIComponent->OnCurrentMpChanged.Broadcast(MpRatio);
	}
}

void URPGAttributeSet::ApplyEquipmentStats(float AddMaxHp, float AddMaxMp, float AddAttackRate, float AddDefense)
{
	// 장비 능력치 적용
	SetMaxHp(GetMaxHp() + AddMaxHp);
	SetMaxMp(GetMaxMp() + AddMaxMp);
	SetAttackRate(GetAttackRate() + AddAttackRate);
	SetDefense(GetDefense() + AddDefense);

	// UI 업데이트
	if (!UIInterface.IsValid()) return;

	UUIComponentBase* const UIComponent = UIInterface->GetUIComponent();
	if (IsValid(UIComponent))
	{
		const float HpRatio = GetCurrentHp() / GetMaxHp();
		UIComponent->OnCurrentHpChanged.Broadcast(HpRatio);
	}
	
	UPlayerUIComponent* const PlayerUIComponent = UIInterface->GetPlayerUIComponent();
	if (IsValid(PlayerUIComponent))
	{
		const float MpRatio = GetCurrentMp() / GetMaxMp();
		PlayerUIComponent->OnCurrentMpChanged.Broadcast(MpRatio);
	}
}

void URPGAttributeSet::RemoveEquipmentStats(float RemoveMaxHp, float RemoveMaxMp, float RemoveAttackRate, float RemoveDefense)
{
	// 장비 능력치 제거
	SetMaxHp(GetMaxHp() - RemoveMaxHp);
	SetMaxMp(GetMaxMp() - RemoveMaxMp);
	SetAttackRate(GetAttackRate() - RemoveAttackRate);
	SetDefense(GetDefense() - RemoveDefense);

	// UI 업데이트
	if (!UIInterface.IsValid()) return;

	UUIComponentBase* const UIComponent = UIInterface->GetUIComponent();
	if (IsValid(UIComponent))
	{
		const float HpRatio = GetCurrentHp() / GetMaxHp();
		UIComponent->OnCurrentHpChanged.Broadcast(HpRatio);
	}
	
	UPlayerUIComponent* const PlayerUIComponent = UIInterface->GetPlayerUIComponent();
	if (IsValid(PlayerUIComponent))
	{
		const float MpRatio = GetCurrentMp() / GetMaxMp();
		PlayerUIComponent->OnCurrentMpChanged.Broadcast(MpRatio);
	}
}

void URPGAttributeSet::AddGameplayTagToActor(AActor* TargetActor, FGameplayTag AddTag)
{
	if (!IsValid(TargetActor)) return;

	if (!AddTag.IsValid()) return;

	URPGAbilitySystemComponent* const ASC = CastChecked<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));

	// 이미 태그가 있는지 확인 후 추가
	const bool bAlreadyHasTag = ASC->HasMatchingGameplayTag(AddTag);
	if (!bAlreadyHasTag)
	{
		ASC->AddLooseGameplayTag(AddTag);
	}
}

void URPGAttributeSet::RemoveGameplayTagFromActor(AActor* TargetActor, FGameplayTag RemoveTag)
{
	if (!IsValid(TargetActor)) return;

	if (!RemoveTag.IsValid()) return;
	
	URPGAbilitySystemComponent* const ASC = CastChecked<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));

	// 태그가 있는지 확인 후 제거
	const bool bHasTag = ASC->HasMatchingGameplayTag(RemoveTag);
	if (bHasTag)
	{
		ASC->RemoveLooseGameplayTag(RemoveTag);
	}
}