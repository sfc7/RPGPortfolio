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

	for (auto& PoolPair : ProjectilePools)
	{
		CreatePool(PoolPair.Key, PoolPair.Value);
	}
}

APooledActor* UObjectPoolComponent::SpawnFromPool(FGameplayTag PoolTag, FVector ObjectLocation, FRotator ObjectRotation)
{
	APooledActor* AvailableActor = FindAvailableActor(PoolTag);
	if (AvailableActor)
	{
		AvailableActor->SetActorLocation(ObjectLocation);
		AvailableActor->SetActorRotation(ObjectRotation);
		AvailableActor->SetIsUse(true);
	}
	return AvailableActor;
}

void UObjectPoolComponent::CreatePool(FGameplayTag PoolTag, FProjectilePoolData& PoolData)
{
	if (!PoolData.ProjectileClass)
	{
		return;
	}

	for (int32 i = 0; i < PoolData.PoolSize; i++)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = Cast<APawn>(GetOwner());

		APooledActor* NewActor = GetWorld()->SpawnActor<APooledActor>(
			PoolData.ProjectileClass, 
			FVector::ZeroVector, 
			FRotator::ZeroRotator, 
			SpawnParams
		);

		if (NewActor)
		{
			NewActor->SetIsUse(false);
			NewActor->SetTimeToLive(PoolData.TimeToLive);
			PoolData.ObjectPool.Add(NewActor);
		}
	}
}

APooledActor* UObjectPoolComponent::FindAvailableActor(FGameplayTag PoolTag)
{
	if (!ProjectilePools.Contains(PoolTag))
	{
		return nullptr;
	}

	for (APooledActor* Actor : ProjectilePools[PoolTag].ObjectPool)
	{
		if (Actor && !Actor->GetIsUse())
		{
			return Actor;
		}
	}
	return nullptr;
}

