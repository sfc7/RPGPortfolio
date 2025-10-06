// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/Skill/PlayerAttackSkillGameplayAbility.h"
#include "RPGGA_Player_Meteor.generated.h"

class AProjectileBase;
/**
 * URPGGA_Player_Meteor
 * 
 * 플레이어 메테오 스킬 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_Meteor : public UPlayerAttackSkillGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_Meteor();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	//@ 스킬 실행 Montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* AttackMontage;

	//@ 어빌리티 종료 콜백 함수
	UFUNCTION()
	void OnEndAbilityCallback();

	//@ 발사체 생성
	UFUNCTION()
	void SpawnProjectile(FGameplayEventData PayloadData);

	//@ 발사체 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AProjectileBase> ProjectileClass;
};