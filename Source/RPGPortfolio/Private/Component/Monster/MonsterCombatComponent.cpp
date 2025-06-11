// LJS


#include "Component/Monster/MonsterCombatComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "RPGFunc.h"
#include "Character/MonsterCharacter.h"
#include "Components/BoxComponent.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"

void UMonsterCombatComponent::OnHitTargetActor(AActor* _HitActor)
{
	if (OverlappedActors.Contains(_HitActor))
	{
		return;
	}

	OverlappedActors.AddUnique(_HitActor);

	bool bIsFacing = false;

	URPGAbilitySystemComponent* HitActorASC = CastChecked<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(_HitActor));
	const bool bIsPlayerDefensing = HitActorASC->HasMatchingGameplayTag(RPGGameplayTag::Player_Status_Defense);

	URPGAbilitySystemComponent* OwnerActorASC = CastChecked<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningPawn()));
	bool bIsUnDefendableAttack = OwnerActorASC->HasMatchingGameplayTag(RPGGameplayTag::Monster_Status_IsBeingUndefendableAttacked);	


	if (bIsPlayerDefensing && !bIsUnDefendableAttack)
	{
		bIsFacing = URPGFunc::IsValidDefense(GetOwningPawn(), _HitActor);
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = _HitActor;
	
	if (bIsFacing)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			_HitActor,
			RPGGameplayTag::Player_Event_DefenseSuccess,
			EventData
		);
	}
	else
	{		
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			RPGGameplayTag::Character_Event_AttackHit_Melee,
			EventData
		);
	}
}

void UMonsterCombatComponent::OnWeaponPulledFromTargetActor(AActor* _InteractedActor, float _WeaponBaseDamage, EWeaponAttackType AttackType)
{
	
}

void UMonsterCombatComponent::ToggleBodyCollisionBoxCollsion(bool _bShouldEnable, EToggleDamageType _ToggleDamageType)
{
	AMonsterCharacter* OwningMonsterCharacter = GetOwningPawn<AMonsterCharacter>();

	if (OwningMonsterCharacter)
	{
		UBoxComponent* LeftHandCollsionBox = OwningMonsterCharacter->GetLeftHandCollisionBox();
		UBoxComponent* RightHandCollsionBox = OwningMonsterCharacter->GetRightHandCollisionBox();

		if (LeftHandCollsionBox && RightHandCollsionBox)
		{
			FString EnumName = UEnum::GetValueAsString(_ToggleDamageType);
			UE_LOG(LogTemp, Warning, TEXT("ToggleDamageType: %s"), *EnumName);
			
			switch (_ToggleDamageType)
			{
			case EToggleDamageType::LeftHand:
				if (_bShouldEnable) LeftHandCollsionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				else LeftHandCollsionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				break;
			case EToggleDamageType::RightHand:
				if (_bShouldEnable) RightHandCollsionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				else RightHandCollsionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				break;
			default:
				break;
			}
		}

		if (!_bShouldEnable)
		{
			OverlappedActors.Empty();
		}
	}

	
}
