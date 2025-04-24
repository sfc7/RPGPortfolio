// LJS


#include "Character/RPGCharacterBase.h"
#include "GameAbilitySystem/MainAbilitySystemComponent.h"
#include "GameAbilitySystem/MainAttributeSet.h"

// Sets default values
ARPGCharacterBase::ARPGCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled =false;
	
	GetMesh()->bReceivesDecals = false;

	MainAbilitySystemComponent = CreateDefaultSubobject<UMainAbilitySystemComponent>(TEXT("MainAbilitySystemComponent"));
	MainAttributeSet = CreateDefaultSubobject<UMainAttributeSet>(TEXT("MainAttributeSet"));
}

UAbilitySystemComponent* ARPGCharacterBase::GetAbilitySystemComponent() const
{
	return GetMainAbilitySystemComponent();
}

void ARPGCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsValid(MainAbilitySystemComponent))
	{
		MainAbilitySystemComponent->InitAbilityActorInfo(this, this);
		
	}
}

