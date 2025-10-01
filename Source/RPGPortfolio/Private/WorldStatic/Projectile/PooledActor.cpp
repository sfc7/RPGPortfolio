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
	// 사용 상태 업데이트, 
	IsUse = NewIsUse;
	SetActorEnableCollision(NewIsUse);
	SetActorHiddenInGame(!NewIsUse);

	// 기존 타이머가 활성화되어 있으면 클리어
	if (GetWorld()->GetTimerManager().IsTimerActive(LiveTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(LiveTimerHandle);
	}
	
	// 사용 상태로 설정된 경우 생존 타이머 시작
	if (NewIsUse)
	{
		GetWorld()->GetTimerManager().SetTimer(LiveTimerHandle, [this]()
		{
		SetIsUse(false);
		}, TimeToLive, false);
	}
}
