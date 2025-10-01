// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AOEBase.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

/**
 * AAOEBase
 * 
 * AOE(Area of Effect)용 기본 클래스
 */
UCLASS()
class RPGPORTFOLIO_API AAOEBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AAOEBase();

	//@ 데칼 스폰 (나이아가라 이펙트 생성)
	UFUNCTION(BlueprintCallable)
	void SpawnDecal();

	//@ 지면 추적용 함수
	UFUNCTION(BlueprintCallable)
	void MoveToFloor();

	//@ 공격 타겟 설정
	UFUNCTION(BlueprintCallable)
	void SetAttackTarget(AActor* TargetActorToSet);
		
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	AActor* TargetActor;

	//@ 데칼 나이아가라 시스템
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UNiagaraSystem* DecalNiagara;

	//@ 나이아가라 컴포넌트 (스폰된 이펙트)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UNiagaraComponent* NiagaraComponent;

	//@ 나이아가라 시스템 완료 이벤트 핸들러
	UFUNCTION()
	void OnNiagaraFinished(UNiagaraComponent* PSystem);
	
public:	
	virtual void Tick(float DeltaTime) override;

};
