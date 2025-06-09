// LJS


#include "WorldStatic/Projectile/ProjectileBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "RPGFunc.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Kismet/GameplayStatics.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollisionBox"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	CollisionComponent->OnComponentHit.AddUniqueDynamic(this, &ThisClass::OnProjectileHit);
	CollisionComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnProjectileBeginOverlap);
	
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

	if (ProjectilePolicy == EProjectilePolicy::BeinOverlap)
	{
		CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
	
}

void AProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ImpactProjectile(Hit.ImpactPoint);
	
	APawn* HitTarget = Cast<APawn>(OtherActor);

	if (!HitTarget || !URPGFunc::CheckTargetTeamAgent(GetInstigator(), HitTarget))
	{
		Destroy();
		return;
	}

	bool bIsValidDefense = false;
	bool bIsPlayerDefensing = false;

	URPGAbilitySystemComponent* TargetASC = Cast<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitTarget));
	if (TargetASC)
	{
		if (TargetASC->HasMatchingGameplayTag(RPGGameplayTag::Player_Status_Defense))
		{
			FGameplayEventData EventData;
			EventData.Instigator = this;
			EventData.Target = HitTarget;
			
			if (URPGFunc::IsValidDefense(this, HitTarget))
			{
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
					HitTarget,
					RPGGameplayTag::Player_Event_DefenseSuccess,
					EventData
					);
			}
			else
			{
				HandleApplyProjectileDamage(HitTarget, EventData);
			}
		}
	}


	Destroy();
	
}

void AProjectileBase::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AProjectileBase::ImpactProjectile(const FVector& HitLocation)
{
	UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, HitLocation);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, HitLocation, FRotator::ZeroRotator, FVector::OneVector,
		true,true, ENCPoolMethod::None, true);
}

void AProjectileBase::HandleApplyProjectileDamage(APawn* HitPawn, const FGameplayEventData& Payload)
{
	check(DamageEffectSpecHandle.IsValid());
	
	const bool bApply = URPGFunc::ApplyGameplayEffectSpecHandleToTargetActor(GetInstigator(), HitPawn, DamageEffectSpecHandle);

	if (bApply)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitPawn,
			RPGGameplayTag::Character_Event_HitReact,
			Payload
			);
	}
}
