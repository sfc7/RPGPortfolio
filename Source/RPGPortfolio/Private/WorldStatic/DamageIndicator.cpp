// LJS


#include "WorldStatic/DamageIndicator.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

ADamageIndicator::ADamageIndicator()
{
	PrimaryActorTick.bCanEverTick = true;

	DamageWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("DamageWidgetComponent");
	DamageWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
}


void ADamageIndicator::BeginPlay()
{
	Super::BeginPlay();

	SetWidgetLocation();
}

void ADamageIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADamageIndicator::SetWidgetLocation()
{
	StartPosition = GetActorLocation();
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter)
	{
		FVector PlayerRandomUpVector = PlayerCharacter->GetActorUpVector() * FMath::RandRange(40.f, 50.f);
		FVector PlayerRandomRightVector = PlayerCharacter->GetActorRightVector() * FMath::RandRange(20.f, 30.f);

		EndPosition = StartPosition + PlayerRandomUpVector + PlayerRandomRightVector;
	}
}

void ADamageIndicator::SetDamage(float Damage)
{
	ShowDamage = Damage;
}
