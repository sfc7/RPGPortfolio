// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawningVolume.generated.h"

class UBoxComponent;
class ATargetPoint;

/**
 * ASpawningVolume
 * 
 * 스포닝 볼륨, 게임 던전 매니저의 데이터 드리븐 구조를 통해 사용됨
 */
UCLASS()
class RPGPORTFOLIO_API ASpawningVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawningVolume();

	//@ 스폰 볼륨 박스 가져오기
	UBoxComponent* GetSpawningBox() const { return SpawningBox; }

	//@ 볼륨 이름 가져오기
	FString GetVolumeName() const { return VolumeName; }

	//@ 박스 영역 내 랜덤 스폰 위치 생성
	UFUNCTION(BlueprintPure)
	FVector GetRandomSpawnLocation();

	//@ 월드 좌표로 액터 스폰
	UFUNCTION(BlueprintCallable)
	AActor* SpawnActorAtWorldLocation(TSubclassOf<AActor> ActorClass, const FVector& Location);
	
	//@ 박스 영역 내 랜덤 위치에 액터 스폰
	UFUNCTION(BlueprintCallable)
	AActor* RandomSpawnActorInBox(TSubclassOf<AActor> ActorClass);

	//@ 박스 로컬 좌표에 액터 스폰 
	UFUNCTION(BlueprintCallable)
	AActor* SpawnActorAtBoxLocalLocation(TSubclassOf<AActor> ActorClass, const FVector& BoxLocalLocation);

protected:
	virtual void BeginPlay() override;

private:
	//@ 스폰 박스 컴포넌트, 스폰 영역 정의용
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* SpawningBox;
	
	//@ 스폰 볼륨 이름 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString VolumeName;
};
