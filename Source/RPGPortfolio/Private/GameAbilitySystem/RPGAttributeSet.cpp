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
	if (!UIInterface.IsValid())
	{
		UIInterface = TWeakInterfacePtr<IUIInterface>(Data.Target.GetAvatarActor());
	}

	check(UIInterface.IsValid());

	UUIComponentBase* UIComponent = UIInterface->GetUIComponent();

	check(UIComponent);
		
	if (Data.EvaluatedData.Attribute == GetCurrentHpAttribute())
	{
		float NewCurrentHp = FMath::Clamp(GetCurrentHp(), 0.f, GetMaxHp());

		SetCurrentHp(NewCurrentHp);
		
		UIComponent->OnCurrentHpChanged.Broadcast(GetCurrentHp()/GetMaxHp());
	}

	if (Data.EvaluatedData.Attribute == GetCurrentMpAttribute())	
	{
		float NewCurrentMp = FMath::Clamp(GetCurrentMp(), 0.f, GetMaxMp());

		SetCurrentMp(NewCurrentMp);

		if (GetCurrentMp() != GetMaxMp())
		{
			AddGameplayTagToActor(Data.Target.GetAvatarActor(), RPGGameplayTag::Player_Status_AttributeSet_MpNotFull);
			RemoveGameplayTagFromActor(Data.Target.GetAvatarActor(), RPGGameplayTag::Player_Status_AttributeSet_MpFull);
		}
		else
		{
			AddGameplayTagToActor(Data.Target.GetAvatarActor(), RPGGameplayTag::Player_Status_AttributeSet_MpFull);
			RemoveGameplayTagFromActor(Data.Target.GetAvatarActor(), RPGGameplayTag::Player_Status_AttributeSet_MpNotFull);
		}
		
		UPlayerUIComponent* PlayerUIComponent = UIInterface->GetPlayerUIComponent();
		if (PlayerUIComponent)
		{
			PlayerUIComponent->OnCurrentMpChanged.Broadcast(GetCurrentMp()/GetMaxMp());
		}
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		float PreCurrentHp = GetCurrentHp();
		float CalcDamage = GetDamage();

		float CalcCurrentHp = FMath::Clamp(PreCurrentHp - CalcDamage, 0.f, GetMaxHp());

		SetCurrentHp(CalcCurrentHp);
		
		UIComponent->OnCurrentHpChanged.Broadcast(GetCurrentHp()/GetMaxHp());
		
		if (GetCurrentHp() == 0.f)
		{
			AddGameplayTagToActor(Data.Target.GetAvatarActor(),RPGGameplayTag::Character_Status_Dead);
		}

		UMonsterUIComponent* MonsterUIComponent = UIInterface->GetMonsterUIComponent();
		if (MonsterUIComponent)
		{
			AMonsterCharacter* Monster = MonsterUIComponent->GetOwningPawn<AMonsterCharacter>();
			Monster->SpawnDamageIndicator(CalcDamage);
		}
	}
}

void URPGAttributeSet::AddGameplayTagToActor(AActor* TargetActor, FGameplayTag AddTag)
{
	check(TargetActor);

	URPGAbilitySystemComponent* ASC = CastChecked<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));

	if (!ASC->HasMatchingGameplayTag(AddTag))
	{
		ASC->AddLooseGameplayTag(AddTag);
	}
}

void URPGAttributeSet::RemoveGameplayTagFromActor(AActor* TargetActor, FGameplayTag RemoveTag)
{
	check(TargetActor);
	
	URPGAbilitySystemComponent* ASC = CastChecked<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));

	if (ASC->HasMatchingGameplayTag(RemoveTag))
	{
		ASC->RemoveLooseGameplayTag(RemoveTag);
	}
}

TMap<FGameplayAttribute, float> URPGAttributeSet::SaveAllAttributes()
{
	TMap<FGameplayAttribute, float> LocalMap;

	TArray<FGameplayAttribute> LocalAttributeArray;
	GetOwningAbilitySystemComponent()->GetAllAttributes(LocalAttributeArray);

	if (!LocalAttributeArray.IsEmpty())
	{
		for (FGameplayAttribute Attribute : LocalAttributeArray)
		{
			bool bFoundAttribute = false;
			float AttributeValue = 	GetOwningAbilitySystemComponent()->GetGameplayAttributeValue(Attribute, bFoundAttribute);

			if (bFoundAttribute)
			{
				LocalMap.Add(Attribute, AttributeValue);				
			}
		}
	}

	return LocalMap;
}

void URPGAttributeSet::LoadAllAttributes(TMap<FGameplayAttribute, float> AttributeMap)
{
	for (auto& Attribute : AttributeMap)
	{
		GetOwningAbilitySystemComponent()->ApplyModToAttribute(Attribute.Key, EGameplayModOp::Override, Attribute.Value);
	}
}

TArray<FAttributeSaveData> URPGAttributeSet::SaveAllAttributesToSaveData()
{
	TArray<FAttributeSaveData> SaveDataArray;
    
	TArray<FGameplayAttribute> LocalAttributeArray;
	GetOwningAbilitySystemComponent()->GetAllAttributes(LocalAttributeArray);
    
	for (FGameplayAttribute Attribute : LocalAttributeArray)
	{
		bool bFoundAttribute = false;
		float AttributeValue = GetOwningAbilitySystemComponent()->GetGameplayAttributeValue(Attribute, bFoundAttribute);
        
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
	TMap<FGameplayAttribute, float> AttributeMap = SaveAllAttributes(); 
    
	for (const FAttributeSaveData& SaveData : AttributeSaveData)
	{
		for (auto& Pair : AttributeMap)
		{
			if (Pair.Key.GetName() == SaveData.AttributeName)
			{
				GetOwningAbilitySystemComponent()->ApplyModToAttribute(Pair.Key, EGameplayModOp::Override, SaveData.Value);
				break;
			}
		}
	}
	
	// UIInterface 유효성 확인 및 초기화
	if (!UIInterface.IsValid())
	{
		if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
		{
			if (AActor* AvatarActor = ASC->GetAvatarActor())
			{
				UIInterface = TWeakInterfacePtr<IUIInterface>(AvatarActor);
			}
		}
	}
	
	// UIInterface가 유효한 경우에만 UI 업데이트
	if (UIInterface.IsValid())
	{
		UUIComponentBase* UIComponent = UIInterface->GetUIComponent();
		if (UIComponent)
		{
			UIComponent->OnCurrentHpChanged.Broadcast(GetCurrentHp()/GetMaxHp());
		}
		
		UPlayerUIComponent* PlayerUIComponent = UIInterface->GetPlayerUIComponent();
		if (PlayerUIComponent)
		{
			PlayerUIComponent->OnCurrentMpChanged.Broadcast(GetCurrentMp()/GetMaxMp());
		}
	}
}

void URPGAttributeSet::AddGameplayTagToOwner(FGameplayTag AddTag)
{
	URPGAbilitySystemComponent* ASC = CastChecked<URPGAbilitySystemComponent>(GetOwningAbilitySystemComponent());

	if (!ASC->HasMatchingGameplayTag(AddTag))
	{
		ASC->AddLooseGameplayTag(AddTag);
	}
}

void URPGAttributeSet::RemoveGameplayTagFromOwner(FGameplayTag RemoveTag)
{
	URPGAbilitySystemComponent* ASC = CastChecked<URPGAbilitySystemComponent>(GetOwningAbilitySystemComponent());

	if (ASC->HasMatchingGameplayTag(RemoveTag))
	{
		ASC->RemoveLooseGameplayTag(RemoveTag);
	}
}

