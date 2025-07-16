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
class ADamageIndicator;
class UParticleSystem;
class UParticleSystemComponent;
struct FPathFollowingResult;
struct FAIRequestID;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTeleportEnd);
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

	void SpawnDamageIndicator(float Damage);

	void Teleport(FVector TargetLocation,AActor* TargetActor);

	void TeleportEnd();

	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);

	FOnTeleportEnd OnTeleportEnd;
protected:
	virtual void BeginPlay() override;
	
	virtual void PossessedBy(AController* NewController) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty( struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UMonsterCombatComponent* MonsterCombatComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UMonsterUIComponent* MonsterUIComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UWidgetComponent* MonsterHpWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DamageIndicator")
	TSubclassOf<ADamageIndicator> DamageIndicator;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* LeftHandCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName LeftHandBoneName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* RightHandCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName RightHandBoneName;

	UFUNCTION()
	virtual void OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystem* TeleportBody;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystemComponent* TeleportBodyComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystem* TeleportTrail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystemComponent* TeleportTrailComponent;
private:
	void InitEnemyStartUpData();


};
