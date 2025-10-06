// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "PlayerGameplayAbility.generated.h"

class APlayerCharacterBase;
class APlayerCharacterController;
class UPlayerCombatComponent;
class UPlayerUIComponent;
/**
 * UPlayerGameplayAbility
 * 
* 플레이어용 기본 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API UPlayerGameplayAbility : public URPGGameplayAbility
{
	GENERATED_BODY()
public:
	//@ 어빌리티 액터 인포에서 플레이어 캐릭터 가져오기
	UFUNCTION(BlueprintPure, Category = "GA")
	APlayerCharacterBase* GetPlayerCharacterFromActorInfo();

	//@ 어빌리티 액터 인포에서 플레이어 컨트롤러 가져오기
	UFUNCTION(BlueprintPure, Category = "GA")
	APlayerCharacterController* GetPlayerControllerFromActorInfo();

	//@ 어빌리티 액터 인포에서 플레이어 CombatComponent 가져오기
	UFUNCTION(BlueprintPure, Category = "GA")
	UPlayerCombatComponent* GetPlayerCombatComponentFromActorInfo();

	//@ 어빌리티 액터 인포에서 플레이어 UIComponent 가져오기
	UFUNCTION(BlueprintPure, Category = "GA")
	UPlayerUIComponent* GetPlayerUIComponentFromActorInfo();

	// BaseDamage를 기본으로 하여 여러 GamePlayTag를 같이 보내서 데미지 최종 계산
	UFUNCTION(BlueprintPure, Category = "GA")
	FGameplayEffectSpecHandle MakePlayerBaseDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> _EffectClass, float _WeaponAttackRate, float _WeaponBaseDamage, FGameplayTag _CurrentAttackTypeTag);
	
	// BaseDamage를 기본으로 하여 여러 GamePlayTag를 같이 보내서 콤보 데미지 최종 계산
	UFUNCTION(BlueprintPure, Category = "GA")
	FGameplayEffectSpecHandle MakePlayerComboDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> _EffectClass, float _WeaponAttackRate, float _WeaponBaseDamage, FGameplayTag _CurrentAttackTypeTag,int32 _UsedComboCount);

	// BaseDamage를 기본으로 하여 여러 GamePlayTag를 같이 보내서 스킬 데미지 최종 계산
	UFUNCTION(BlueprintPure, Category = "GA")
	FGameplayEffectSpecHandle MakePlayerSkillDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> _EffectClass, float _WeaponBaseDamage, const FScalableFloat& _SkillDamageMultiplier);

	//@ GamePlayTag로 어빌리티 남은 쿨다운 시간 가져오기
	UFUNCTION(BlueprintCallable, Category = "GA")
	bool GetAbilityRemaningCooldownByTag(FGameplayTag CooldownTag, float& TotalCooldownTime, float& RemaningCooldownTime);

	//@ 스킬 창 UI용 어빌리티 설명 텍스트 생성
	FString GetDescriptionForUI();
	
protected:
	//@ GamePlayEffect로 레벨에 따른 마나 소모량 계산
	float GetCostMana(float InLevel);
	//@ GamePlayEffect로 레벨에 따른 쿨타임 계산
	float GetCooldown(float InLevel);
	
private:
	//@ 캐시된 플레이어 캐릭터 참조
	TWeakObjectPtr<APlayerCharacterBase> PlayerCharacter;
	//@ 캐시된 플레이어 컨트롤러 참조
	TWeakObjectPtr<APlayerCharacterController> PlayerCharacterController;

	
};
