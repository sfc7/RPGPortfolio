// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/Skill/PlayerAttackSkillGameplayAbility.h"
#include "RPGGA_Player_TeleportDropKick.generated.h"

/**
 * URPGGA_Player_TeleportDropKick
 * 
 * 플레이어 드롭킥 스킬 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_TeleportDropKick : public UPlayerAttackSkillGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_TeleportDropKick();

	//@ 모션 워핑의 회전에 사용할 FName
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName WarpTargetNameDirection;

	//@ 모션 워핑의 거리 값에 사용할 FName
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName WarpTargetNameDistance;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	//@ 캐릭터 정방향 값(주위에 적이 없을시 사용할 함수)
	UFUNCTION()
	void ForwardDirection();

	//@ 캐릭터가 회전한 값(주위에 적이 있을시 사용할 함수)
	UFUNCTION()
	void RotateDirection();

	//@ 어빌리티 종료 콜백 함수
	UFUNCTION()
	void OnEndAbilityCallback();

	//@ 드롭킥 Montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* AttackMontage;

	//@ 타겟에게 GameplayEffect 적용(데미지, 이펙트 등)
	UFUNCTION()
	void ApplyEffectsSpecHandleToTargetCallback(FGameplayEventData PayloadData);


	
};
