// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AOEBase.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class RPGPORTFOLIO_API AAOEBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AAOEBase();

	UFUNCTION(BlueprintCallable)
	void SpawnDecal();

	UFUNCTION(BlueprintCallable)
	void MoveToFloor();

	UFUNCTION(BlueprintCallable)
	void SetAttackTarget(AActor* TargetActorToSet);
		
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	AActor* TargetActor;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UNiagaraSystem* DecalNiagara;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UNiagaraComponent* NiagaraComponent;

	UFUNCTION()
	void OnNiagaraFinished(UNiagaraComponent* PSystem);
	
public:	
	virtual void Tick(float DeltaTime) override;

};
