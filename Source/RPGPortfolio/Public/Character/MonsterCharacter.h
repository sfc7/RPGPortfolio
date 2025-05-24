// LJS

#pragma once

#include "CoreMinimal.h"
#include "Character/RPGCharacterBase.h"
#include "NiagaraComponent.h"
#include "MonsterCharacter.generated.h"

class UMonsterCombatComponent;
class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API AMonsterCharacter : public ARPGCharacterBase
{
	GENERATED_BODY()
public:
	AMonsterCharacter();

	FORCEINLINE UMonsterCombatComponent* GetMonsterCombatComponent() const { return MonsterCombatComponent; }
	
	virtual UCombatComponentBase* GetCombatComponent() const override;

	virtual void MonsterDeath(TSoftObjectPtr<UNiagaraSystem> _DeathNiagaraEffectSoftObject);
protected:

	virtual void PossessedBy(AController* NewController) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMonsterCombatComponent* MonsterCombatComponent;

private:
	void InitEnemyStartUpData();
	
};
