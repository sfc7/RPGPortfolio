// LJS


#include "WorldStatic/AOE/AOEBase.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
AAOEBase::AAOEBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AAOEBase::BeginPlay()
{
	Super::BeginPlay();

	SpawnDecal();	
}

void AAOEBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAOEBase::SpawnDecal()
{	
	if (DecalNiagara)
	{
		NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DecalNiagara, GetActorLocation(), FRotator::ZeroRotator, FVector::OneVector,
	true,true, ENCPoolMethod::None, true);

		if (NiagaraComponent)
		{
			NiagaraComponent->SetAutoDestroy(true); 
			NiagaraComponent->OnSystemFinished.AddDynamic(this, &AAOEBase::OnNiagaraFinished);
		}
	}

	MoveToFloor();
}

void AAOEBase::OnNiagaraFinished(UNiagaraComponent* PSystem)
{
	Destroy();
}

void AAOEBase::MoveToFloor()
{
	if (TargetActor)
	{
		FVector TargetActorLocation = GetActorLocation();
		FVector EndLocation = TargetActorLocation + FVector(0.f, 0.f, -500.f);
		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.bTraceComplex = false;   
		QueryParams.AddIgnoredActor(TargetActor);
		QueryParams.AddIgnoredActor(this);
		
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			OUT HitResult,
			TargetActorLocation,
			EndLocation,
			ECC_Visibility,
			QueryParams
			);

		if (bHit)
		{
			SetActorLocation(HitResult.Location);
		}
	}
}

void AAOEBase::SetAttackTarget(AActor* TargetActorToSet)
{
	TargetActor = TargetActorToSet;
}



