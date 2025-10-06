// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Monster/MonsterGameplayAbility.h"
#include "RPGGA_Monster_Attack_Melee.generated.h"

/**
 * URPGGA_Monster_Attack_EarthQuake
 * 
 * 몬스터 기본 근접 공격 패턴 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Monster_Attack_Melee : public UMonsterGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Monster_Attack_Melee();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	//@ 어빌리티 종료 콜백 함수
	UFUNCTION()
	void OnEndAbilityCallback();

	//@ 데미지 적용
	UFUNCTION()
	void ApplyDamage(FGameplayEventData PayloadData);

	//@ 데미지 계산용 이펙트 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	//@ 공격 적중 시 발생하는 사운드 GameplayCue를 실행 할 GameplayTag
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag HitSoundGameplayCueTag;

	//@ 데미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FScalableFloat DamageScale;

	//@ 타겟
	AActor* LocalTargetActor;

	//@ 공격 Montages
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UAnimMontage*> AttackMontages;

	
};
