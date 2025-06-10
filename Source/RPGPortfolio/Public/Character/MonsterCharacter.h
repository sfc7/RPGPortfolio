// LJS

#pragma once

#include "CoreMinimal.h"
#include "Character/RPGCharacterBase.h"
#include "NiagaraComponent.h"
#include "MonsterCharacter.generated.h"

class UMonsterCombatComponent;
class UNiagaraSystem;
class UMonsterUIComponent;
class UWidgetComponent;
class UBoxComponent;
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
	
	FORCEINLINE UBoxComponent* GetLeftHandCollisionBox() const { return LeftHandCollisionBox; }

	FORCEINLINE UBoxComponent* GetRightHandCollisionBox() const { return RightHandCollisionBox; }

	
	virtual UCombatComponentBase* GetCombatComponent() const override;

	virtual void MonsterDeath(TSoftObjectPtr<UNiagaraSystem> _DeathNiagaraEffectSoftObject);

	UFUNCTION()
	void OnDeathEffectFinished(UNiagaraComponent* FinishedComponent);
	
	virtual UUIComponentBase* GetUIComponent() const override;

	virtual UMonsterUIComponent* GetMonsterUIComponent() const override;
protected:
	virtual void BeginPlay() override;
	
	virtual void PossessedBy(AController* NewController) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UMonsterCombatComponent* MonsterCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UBoxComponent* LeftHandCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UBoxComponent* RightHandCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UMonsterUIComponent* MonsterUIComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UWidgetComponent* MonsterHpWidgetComponent;

	UFUNCTION()
	virtual void OnBodyCollisionBoxBeginOverlap();

private:
	void InitEnemyStartUpData();
	
};
