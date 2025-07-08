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

	InitializePool();
}


void UObjectPoolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}



void UObjectPoolComponent::InitializePool()
{
	for (int index = 0; index < PoolSize; index++)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = Cast<APawn>(GetOwner());
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		APooledActor* SpawnProjectile = GetWorld()->SpawnActor<APooledActor>(PooledActorClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		SpawnProjectile->SetIsUse(false);
		SpawnProjectile->SetTimeToLive(TimeToLive);
		ObjectPool.AddUnique(SpawnProjectile);
	}
}

APooledActor* UObjectPoolComponent::FindAvailableActor()
{
	for (APooledActor* PoolActor : ObjectPool)
	{
		if (!PoolActor->GetIsUse())
		{
			return PoolActor;
		}
	}

	return nullptr;
}

APooledActor* UObjectPoolComponent::SpawnFromPool(FVector ObjectLocation, FRotator ObjectRotation)
{
	APooledActor* AvailableActor = FindAvailableActor();
	if (IsValid(AvailableActor))
	{
		AvailableActor->SetActorLocation(ObjectLocation);
		AvailableActor->SetActorRotation(ObjectRotation);		
		AvailableActor->SetIsUse(true);
		return AvailableActor;
	}
	return nullptr;
}
