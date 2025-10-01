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
	if (!IsValid(ProjectileMovementComponent)) return;
	

	// 이동 중지 및 컴포넌트 설정
	ProjectileMovementComponent->StopMovementImmediately();
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	
	// 발사 방향과 속도 설정
	ProjectileMovementComponent->Velocity = LaunchDirection.GetSafeNormal() * LaunchSpeed;
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	// 오버랩 정책인 경우 콜리전 응답 변경
	if (ProjectilePolicy == EProjectilePolicy::BeinOverlap)
	{
		CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}

	// 발사자 무시 설정
	if (GetInstigator())
	{
		CollisionComponent->IgnoreActorWhenMoving(GetInstigator(), true);
	}
}

void AProjectileBase::SetIsUse(bool NewIsUse)
{
	Super::SetIsUse(NewIsUse);

	if (!IsValid(ProjectileMovementComponent)) return;
		
	// 사용 플래그 따라서 Projectile 활성화
	if (NewIsUse)
	{
		// 사용 시 이펙트 활성화
		NiagaraComponent->Activate();
        
		// 프로젝타일 이동 활성화
		ProjectileMovementComponent->Activate(true);
		
	}
	else
	{
		// 비사용 시 이펙트 비활성화
		NiagaraComponent->Deactivate();
		
		// 프로젝타일 이동 비활성화
		ProjectileMovementComponent->SetComponentTickEnabled(false);
	}
}

void AProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == GetInstigator()) return; 
	
	if (Cast<AProjectileBase>(OtherActor)) return;

	// 임팩트 이펙트 재생
	ImpactProjectile(Hit.ImpactPoint);
	
	APawn* HitTarget = Cast<APawn>(OtherActor);
	
	// 적대적인 타겟인지 확인
	if (!HitTarget || !URPGFunc::CheckTargetTeamAgent(GetInstigator(), HitTarget))
	{
		SetIsUse(false);
		return;
	}

	// 타겟의 ASC를 통해 타겟의 상태를 확인하고 상태에 따라 처리
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
	// 임팩트 사운드 재생
	UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, HitLocation);

	// 나이아가라 이펙트 재생
	if (ImpactNiagaraEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactNiagaraEffect, HitLocation, FRotator::ZeroRotator, FVector::OneVector,
	true,true, ENCPoolMethod::None, true);
	}
	
	// 파티클 이펙트 재생
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

	// GAS 매니저를 통한 데미지 적용
	const bool bApply = GetGameInstance()->GetSubsystem<UGASManager>()->ApplyGameplayEffectSpecHandleToTargetActor(GetInstigator(), HitPawn, DamageEffectSpecHandle);

	// 데미지 적용 성공 시 히트 GameplayEvent 전송
	if (bApply)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitPawn,
			RPGGameplayTag::Character_Event_HitReact,
			Payload
			);
	}
}
