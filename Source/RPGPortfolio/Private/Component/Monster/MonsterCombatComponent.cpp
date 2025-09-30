// LJS


#include "Component/Monster/MonsterCombatComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "RPGFunc.h"
#include "Character/MonsterCharacter.h"
#include "Components/BoxComponent.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"


void UMonsterCombatComponent::RegisterSpawnedWeapon(FGameplayTag _WeaponTagToRegister, AWeaponBase* _WeaponToRegister, bool bEquippedWeaponState)
{
	Super::RegisterSpawnedWeapon(_WeaponTagToRegister, _WeaponToRegister, bEquippedWeaponState);
}

void UMonsterCombatComponent::OnHitTargetActor(AActor* _HitActor, float WeaponBaseDamage, float WeaponAttackRate, EWeaponAttackType AttackType, EWeaponType WeaponType, FName SocketName)
{
	if (!IsValid(_HitActor))
	{
		return;
	}
	
	UAbilitySystemComponent* HitActorASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(_HitActor);

	if (!IsValid(HitActorASC))
	{
		return;
	}

	// 무적 상태 처리
	if (HitActorASC->HasMatchingGameplayTag(RPGGameplayTag::Character_Status_Invincible))
	{
		return; 
	}

	// 중복 히트 방지
	if (OverlappedActors.Contains(_HitActor))
	{
		return;
	}

	// 히트된 액터를 중복 히트 방지 목록에 추가
	OverlappedActors.AddUnique(_HitActor);

	// 회피와 디펜스 체크
	const bool bIsPlayerDodging = HitActorASC->HasMatchingGameplayTag(RPGGameplayTag::Player_Status_ActionState_IsDodging);
	const bool bIsPlayerDefensing = HitActorASC->HasMatchingGameplayTag(RPGGameplayTag::Player_Status_ActionState_IsDefensing);
	
	URPGAbilitySystemComponent* OwnerActorASC = CastChecked<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningPawn()));
	// 방어 불가 공격 체크
	const bool bIsUnDefendableAttack = OwnerActorASC->HasMatchingGameplayTag(RPGGameplayTag::Monster_Status_IsBeingUndefendableAttacked);	

	// 방어 성공 각도 체크
	bool bIsFacing = false;
	if (bIsPlayerDefensing && !bIsUnDefendableAttack)
	{
		bIsFacing = URPGFunc::IsValidDefense(GetOwningPawn(), _HitActor);
	}

	// 공격에 대해 체크한 로직에 따라 GameplayEventData 전송
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
	else if (bIsPlayerDodging)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			_HitActor,
			RPGGameplayTag::Player_Event_DodgeSuccess,
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


void UMonsterCombatComponent::OnWeaponPulledFromTargetActor(AActor* _HitActor, float WeaponBaseDamage, float WeaponAttackRate, EWeaponAttackType AttackType, EWeaponType WeaponType)
{
	
}

void UMonsterCombatComponent::ToggleBodyCollisionBoxCollsion(bool _bShouldEnable, EToggleDamageType _ToggleDamageType)
{
	const AMonsterCharacter* OwningMonsterCharacter = GetOwningPawn<AMonsterCharacter>();
	if (!IsValid(OwningMonsterCharacter))
	{
		return;
	}
	
	UBoxComponent* LeftHandCollisionBox = OwningMonsterCharacter->GetLeftHandCollisionBox();
	UBoxComponent* RightHandCollisionBox = OwningMonsterCharacter->GetRightHandCollisionBox();
	
	if (!IsValid(LeftHandCollisionBox) || !IsValid(RightHandCollisionBox))
	{
		return;
	}

	// 토글 타입에 따른 충돌 상태 설정
	switch (_ToggleDamageType)
	{
	case EToggleDamageType::LeftHand:
		if (_bShouldEnable)
		{
			LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		break;

	case EToggleDamageType::RightHand:
		if (_bShouldEnable)
		{
			RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		break;

	default:
		break;
	}

	// 충돌 비활성화 시 겹친 액터 목록 초기화
	if (!_bShouldEnable)
	{
		OverlappedActors.Empty();
	}
}
