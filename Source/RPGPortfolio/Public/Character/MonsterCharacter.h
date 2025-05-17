// LJS

#pragma once

#include "CoreMinimal.h"
#include "Character/RPGCharacterBase.h"
#include "MonsterCharacter.generated.h"

class UMonsterCombatComponent;

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

protected:

	virtual void PossessedBy(AController* NewController) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMonsterCombatComponent* MonsterCombatComponent;

private:
	void InitEnemyStartUpData();
	
};
