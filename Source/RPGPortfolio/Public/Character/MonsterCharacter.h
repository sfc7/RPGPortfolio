// LJS

#pragma once

#include "CoreMinimal.h"
#include "Character/RPGCharacterBase.h"
#include "NiagaraComponent.h"
#include "MonsterCharacter.generated.h"

class UMonsterCombatComponent;
class UNiagaraSystem;
class UMonsterUIComponent;
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

	virtual UUIComponentBase* GetUIComponent() const override;

	virtual UMonsterUIComponent* GetMonsterUIComponent() const override;
protected:

	virtual void PossessedBy(AController* NewController) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UMonsterCombatComponent* MonsterCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UMonsterUIComponent* MonsterUIComponent;

private:
	void InitEnemyStartUpData();
	
};
