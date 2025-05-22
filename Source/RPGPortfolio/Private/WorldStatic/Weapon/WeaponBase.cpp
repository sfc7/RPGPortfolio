// LJS


#include "WorldStatic/Weapon/WeaponBase.h"

#include "RPGStructTypes.h"
#include "Components/BoxComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
 
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
 
	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox"));
	WeaponCollisionBox->SetupAttachment(GetRootComponent());
	WeaponCollisionBox->SetBoxExtent(FVector(20.f));
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxBeginOverlap);
	WeaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxEndOverlap);
}

void AWeaponBase::SetCurrentAttackType(EWeaponAttackType AttackType)
{
	CurrentAttackType = AttackType;

}

void AWeaponBase::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* WeaponOwner = GetInstigator<APawn>();
	
	if (IsValid(WeaponOwner))
	{
		if (APawn* HitPawn = Cast<APawn>(OtherActor))
		{
			if (WeaponOwner != OtherActor)
			{
				OnWeaponHitTarget.ExecuteIfBound(OtherActor, WeaponDefaultData.WeaponBaseDamage, CurrentAttackType);

			}
		}
	}
}

void AWeaponBase::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* WeaponOwner = GetInstigator<APawn>();
	
	if (IsValid(WeaponOwner))
	{
		if (APawn* HitPawn = Cast<APawn>(OtherActor))
		{
			if (WeaponOwner != OtherActor)
			{
				OnWeaponHitTarget.ExecuteIfBound(OtherActor, WeaponDefaultData.WeaponBaseDamage, CurrentAttackType);
			}
		}
	}
}
