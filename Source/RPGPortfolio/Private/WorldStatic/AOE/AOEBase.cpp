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
	if (IsValid(DecalNiagara))
	{
		// 나이아가라 시스템 스폰
		NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DecalNiagara, GetActorLocation(), FRotator::ZeroRotator, FVector::OneVector,
	true,true, ENCPoolMethod::None, true);

		if (IsValid(NiagaraComponent))
		{
			// 자동 파괴 활성화
			NiagaraComponent->SetAutoDestroy(true);
			// 완료 이벤트 바인딩
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
	if (!IsValid(TargetActor)) return;
	
	// 현재 위치에서 아래쪽으로 라인 트레이스
	const FVector TargetActorLocation = GetActorLocation();
	const FVector EndLocation = TargetActorLocation + FVector(0.f, 0.f, -500.f);
	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;
	QueryParams.AddIgnoredActor(TargetActor);
	QueryParams.AddIgnoredActor(this);
	

	const UWorld* World = GetWorld();
	if (!IsValid(World)) return;

	// 라인 트레이스 실행
	const bool bHit = World->LineTraceSingleByChannel(
		OUT HitResult,
		TargetActorLocation,
		EndLocation,
		ECC_Visibility,
		QueryParams
	);

	// 히트된 경우 위치 업데이트
	if (bHit)
	{
		SetActorLocation(HitResult.Location);
	}
}

void AAOEBase::SetAttackTarget(AActor* TargetActorToSet)
{
	TargetActor = TargetActorToSet;
}



