// LJS


#include "WorldStatic/Projectile/PooledActor.h"

APooledActor::APooledActor()
{
	PrimaryActorTick.bCanEverTick = true;

}


void APooledActor::BeginPlay()
{
	Super::BeginPlay();

	SetIsUse(false);
	
}

void APooledActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APooledActor::SetIsUse(bool NewIsUse)
{
	IsUse = NewIsUse;
	SetActorEnableCollision(NewIsUse);
	SetActorHiddenInGame(!NewIsUse);
	
	if (GetWorld()->GetTimerManager().IsTimerActive(LiveTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(LiveTimerHandle);
	}
    
	if (NewIsUse)
	{
		GetWorld()->GetTimerManager().SetTimer(LiveTimerHandle, [this]()
		{
		SetIsUse(false);
		}, TimeToLive, false);
	}
}
