// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "MonsterGameplayAbility.generated.h"

class AMonsterCharacter;
class UMonsterCombatComponent;
/**
 * UMonsterGameplayAbility
 * 
* 몬스터용 기본 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API UMonsterGameplayAbility : public URPGGameplayAbility
{
	GENERATED_BODY()
public:
	//@ 어빌리티 액터 인포에서 몬스터 캐릭터 가져오기
	UFUNCTION(BlueprintPure, Category = "GA")
	AMonsterCharacter* GetMonsterCharacterFromActorInfo();

	//@ 어빌리티 액터 인포에서 몬스터 CombatComponent 가져오기
	UFUNCTION(BlueprintPure, Category = "GA")
	UMonsterCombatComponent* GetMonsterCombatComponentFromActorInfo();

	//@ BaseDamage를 기본으로 하여 여러 GamePlayTag를 같이 보내서 데미지 최종 계산
	UFUNCTION(BlueprintPure, Category = "GA")
	FGameplayEffectSpecHandle MakeMonsterBaseDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> _EffectClass, const FScalableFloat& _DamageScale);

private:
	//@ 캐시된 몬스터 약참조
	TWeakObjectPtr<AMonsterCharacter> MonsterCharacter;
};
