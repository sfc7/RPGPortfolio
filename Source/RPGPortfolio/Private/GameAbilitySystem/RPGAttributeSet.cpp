// LJS


#include "GameAbilitySystem/RPGAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "Component/UIComponentBase.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Component/Player/PlayerUIComponent.h"
#include "Interface/UIInterface.h"


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

