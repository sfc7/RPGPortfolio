// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "MonsterGameplayAbility.generated.h"

class AMonsterCharacter;
class UMonsterCombatComponent;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UMonsterGameplayAbility : public URPGGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "GA")
	AMonsterCharacter* GetMonsterCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "GA")
	UMonsterCombatComponent* GetMonsterCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "GA")
	FGameplayEffectSpecHandle MakeMonsterDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> _EffectClass, const FScalableFloat& _DamageScale);
private:
	TWeakObjectPtr<AMonsterCharacter> MonsterCharacter;
};
