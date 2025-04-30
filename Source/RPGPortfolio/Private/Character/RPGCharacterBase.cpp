// LJS


#include "Character/RPGCharacterBase.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/RPGAttributeSet.h"

// Sets default values
ARPGCharacterBase::ARPGCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled =false;
	
	GetMesh()->bReceivesDecals = false;

	RPGAbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>(TEXT("RPGAbilitySystemComponent"));
	RPGAttributeSet = CreateDefaultSubobject<URPGAttributeSet>(TEXT("RPGAttributeSet"));
}

UAbilitySystemComponent* ARPGCharacterBase::GetAbilitySystemComponent() const
{
	return GetRPGAbilitySystemComponent();
}

void ARPGCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsValid(RPGAbilitySystemComponent))
	{
		RPGAbilitySystemComponent->InitAbilityActorInfo(this, this);
		
	}
}

