// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Monster/MonsterGameplayAbility.h"
#include "RPGGA_Monster_Attack_Meteor.generated.h"

class AAOEBase;
class AProjectileBase;
/**
 * URPGGA_Monster_Attack_Meteor
 * 
 * 보스 몬스터 메테오 패턴 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Monster_Attack_Meteor : public UMonsterGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Monster_Attack_Meteor();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	//@ 어빌리티 종료 콜백 함수
	UFUNCTION()
	void OnEndAbilityCallback();

	//@ 메테오 발사체 생성
	UFUNCTION()
	void SpawnMeteor(FGameplayEventData PayloadData);

	//@ 데미지 계산용 이펙트 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	//@ 데미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FScalableFloat DamageScale;

	//@ 공격 Montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* AttackMontage;

	//@ 메테오 타겟 탐지에 사용할 반지름
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MeteorTraceRadius;

	//@ 메테오 소환 포탈 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* PortalEffect;

	//@ 메테오 랜덤 소환 위치
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MeteorRandomOffset;

	//@ 메테오의 떨어지는 위치를 AOEBase를 통해 데칼로 표현함
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AAOEBase> MeteorDecalClass;

	//@ 발사체 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AProjectileBase> ProjectileClass;

protected:
	//@ 메테오 포격시 필요한 해당 무기를 보여줌
	UFUNCTION()
	void ShowWeapon(FGameplayEventData PayloadData);

	//@ 메테오 포격 종료 시 무기를 다시 숨김 
	UFUNCTION()
	void HideWeapon(FGameplayEventData PayloadData);
};
