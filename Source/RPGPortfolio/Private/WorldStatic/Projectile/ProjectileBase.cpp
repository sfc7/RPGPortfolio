// LJS


#include "WorldStatic/Projectile/ProjectileBase.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollisionBox"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileNiagaraComponent"));
	NiagaraComponent->SetupAttachment(GetRootComponent());

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComponent->InitialSpeed = 700.f;
	ProjectileMovementComponent->MaxSpeed = 900.f;
	ProjectileMovementComponent->Velocity = FVector(1.f,0.f,0.f);
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 4.f;
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}
