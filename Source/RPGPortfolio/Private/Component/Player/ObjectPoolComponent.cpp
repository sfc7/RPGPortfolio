// LJS


#include "Component/Player/ObjectPoolComponent.h"
#include "WorldStatic/Projectile/ProjectileBase.h"
#include "WorldStatic/Projectile/PooledActor.h"
#include "GameFramework/ProjectileMovementComponent.h"

UObjectPoolComponent::UObjectPoolComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UObjectPoolComponent::BeginPlay()
{
	Super::BeginPlay();

	// 모든 프로젝타일 풀 생성
	for (auto& PoolPair : ProjectilePools)
	{
		CreatePool(PoolPair.Key, PoolPair.Value);
	}
}

APooledActor* UObjectPoolComponent::SpawnFromPool(FGameplayTag PoolTag, FVector ObjectLocation, FRotator ObjectRotation)
{
	APooledActor* const AvailableActor = FindAvailableActor(PoolTag);
	if (!IsValid(AvailableActor)) return nullptr;

	// 액터 위치와 회전 설정
	AvailableActor->SetActorLocation(ObjectLocation);
	AvailableActor->SetActorRotation(ObjectRotation);
	
	// 사용 상태로 변경
	AvailableActor->SetIsUse(true);
	
	return AvailableActor;
}

void UObjectPoolComponent::CreatePool(FGameplayTag PoolTag, FProjectilePoolData& PoolData)
{
	if (!IsValid(PoolData.ProjectileClass)) return;

	// 지정된 개수만큼 오브젝트 미리 생성
	for (int32 i = 0; i < PoolData.PoolSize; i++)
	{
		// 스폰 파라미터 설정
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = Cast<APawn>(GetOwner());
		
		APooledActor* const NewActor = GetWorld()->SpawnActor<APooledActor>(
			PoolData.ProjectileClass, 
			FVector::ZeroVector, 
			FRotator::ZeroRotator, 
			SpawnParams
		);

		// 생성된 액터 초기화 및 풀에 추가
		if (IsValid(NewActor))
		{
			// 비활성 상태로 설정
			NewActor->SetIsUse(false);
			// 생존 시간 설정
			NewActor->SetTimeToLive(PoolData.TimeToLive);
			// 풀에 추가
			PoolData.ObjectPool.Add(NewActor);
		}
	}
}

APooledActor* UObjectPoolComponent::FindAvailableActor(FGameplayTag PoolTag)
{
	if (!ProjectilePools.Contains(PoolTag)) return nullptr;

	// 풀에서 사용 가능한 오브젝트 검색
	const TArray<APooledActor*>& ObjectPool = ProjectilePools[PoolTag].ObjectPool;
	for (APooledActor* const Actor : ObjectPool)
	{
		// 유효하고 사용 중이 아닌 오브젝트 찾기
		if (IsValid(Actor) && !Actor->GetIsUse())
		{
			return Actor;
		}
	}
	
	return nullptr;
}

