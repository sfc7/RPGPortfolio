// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PooledActor.generated.h"

/**
 * APooledActor
 * 
 * 오브젝트 풀링을 위한 기본 액터 클래스
 */
UCLASS()
class RPGPORTFOLIO_API APooledActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APooledActor();

	//@ 오브젝트 사용 상태 가져오기
	UFUNCTION(BlueprintCallable)
	bool GetIsUse() const { return IsUse; }

	//@ 오브젝트 생존 시간 가져오기
	UFUNCTION(BlueprintCallable)
	float GetTimeToLive() const { return TimeToLive; }

	//@ 오브젝트 사용 상태 설정
	UFUNCTION(BlueprintCallable)
	virtual void SetIsUse(bool NewIsUse);

	//@ 오브젝트 생존 시간 설정
	UFUNCTION(BlueprintCallable)
	void SetTimeToLive(float NewTimeToLive) { TimeToLive = NewTimeToLive; }
	
protected:
	//@ 사용 상태 플래그
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool IsUse;

	//@ 생존 시간
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TimeToLive;

	//@ 생존 타이머 핸들
	FTimerHandle LiveTimerHandle;
	
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
