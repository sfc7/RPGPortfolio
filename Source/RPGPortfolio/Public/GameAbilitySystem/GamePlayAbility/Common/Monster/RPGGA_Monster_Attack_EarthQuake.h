// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Monster/MonsterGameplayAbility.h"
#include "RPGGA_Monster_Attack_EarthQuake.generated.h"

/**
 * URPGGA_Monster_Attack_EarthQuake
 * 
 * 보스 몬스터 지진 패턴 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Monster_Attack_EarthQuake : public UMonsterGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Monster_Attack_EarthQuake();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	//@ 어빌리티 종료 콜백 함수
	UFUNCTION()
	void OnEndAbilityCallback();

	//@ 지진 패턴에 대한 범위형 공격을 입히는 함수
	UFUNCTION()
	void CheckBoxTraceAndApplyDamgage(FGameplayEventData PayloadData);

	//@ 데미지 계산용 이펙트 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	//@ 땅을 찍을 때 발생하는 사운드 GameplayCue를 실행 할 GameplayTag
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag HitSoundGameplayCueTag;

	//@ 데미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FScalableFloat DamageScale;

	//@ 공격 Montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* AttackMontage;

	//@ 지진 범위
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float EarthQuakeRadius;
};
