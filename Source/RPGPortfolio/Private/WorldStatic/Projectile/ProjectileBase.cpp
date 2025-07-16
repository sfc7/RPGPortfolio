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
#include "GameMode/GameManager/GASManager.h"
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

	CascadeComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ProjectileCascadeComponent"));
	CascadeComponent->SetupAttachment(GetRootComponent());
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->InitialSpeed = 700.f;
	ProjectileMovementComponent->MaxSpeed = 900.f;
	ProjectileMovementComponent->Velocity = FVector(1.f,0.f,0.f);
	// InitialLifeSpan = 4.f;
}

void AProjectileBase::LaunchProjectile(const FVector& LaunchDirection, float LaunchSpeed)
{
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->StopMovementImmediately();
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
        
		ProjectileMovementComponent->Velocity = LaunchDirection.GetSafeNormal() * LaunchSpeed;
	}
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (ProjectilePolicy == EProjectilePolicy::BeinOverlap)
	{
		CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
	
	if (GetInstigator())
	{
		CollisionComponent->IgnoreActorWhenMoving(GetInstigator(), true);
	}
}

void AProjectileBase::SetIsUse(bool NewIsUse)
{
	Super::SetIsUse(NewIsUse);

	if (NewIsUse)
	{
		NiagaraComponent->Activate();
        
		if (ProjectileMovementComponent)
		{
			ProjectileMovementComponent->SetComponentTickEnabled(true);
			ProjectileMovementComponent->Activate(true);
		}
	}
	else
	{
		NiagaraComponent->Deactivate();
			
		if (ProjectileMovementComponent)
		{
			ProjectileMovementComponent->Velocity = FVector::ZeroVector;
			ProjectileMovementComponent->SetComponentTickEnabled(false);
		}
	}
}

void AProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == GetInstigator())
	{
		return; 
	}
	
	if (Cast<AProjectileBase>(OtherActor))
	{
		return;
	}
	
	ImpactProjectile(Hit.ImpactPoint);
	
	APawn* HitTarget = Cast<APawn>(OtherActor);

	if (!HitTarget || !URPGFunc::CheckTargetTeamAgent(GetInstigator(), HitTarget))
	{
		SetIsUse(false);
		return;
	}

	URPGAbilitySystemComponent* TargetASC = Cast<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitTarget));
	if (TargetASC)
	{
		FGameplayEventData EventData;
		EventData.Instigator = this;
		EventData.Target = HitTarget;
		
		if (TargetASC->HasMatchingGameplayTag(RPGGameplayTag::Player_Status_ActionState_IsDefensing))
		{
			if (URPGFunc::IsValidDefense(this, HitTarget))
			{
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
					HitTarget,
					RPGGameplayTag::Player_Event_DefenseSuccess,
					EventData
					);
			}
		}
		else
		{
			HandleApplyProjectileDamage(HitTarget, EventData);
		}
	}

	SetIsUse(false);
}

void AProjectileBase::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AProjectileBase::ImpactProjectile(const FVector& HitLocation)
{
	UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, HitLocation);

	if (ImpactNiagaraEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactNiagaraEffect, HitLocation, FRotator::ZeroRotator, FVector::OneVector,
	true,true, ENCPoolMethod::None, true);
	}

	if (ImpactParticleEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
		this, 
		ImpactParticleEffect,                  
		HitLocation, 
		FRotator::ZeroRotator, 
		FVector::OneVector,          
		true                         
		);
	}
}

void AProjectileBase::HandleApplyProjectileDamage(APawn* HitPawn, const FGameplayEventData& Payload)
{
	check(DamageEffectSpecHandle.IsValid());
	
	const bool bApply = GetGameInstance()->GetSubsystem<UGASManager>()->ApplyGameplayEffectSpecHandleToTargetActor(GetInstigator(), HitPawn, DamageEffectSpecHandle);

	if (bApply)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitPawn,
			RPGGameplayTag::Character_Event_HitReact,
			Payload
			);
	}
}
