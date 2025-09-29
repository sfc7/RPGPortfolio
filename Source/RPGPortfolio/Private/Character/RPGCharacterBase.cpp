// LJS


#include "Character/RPGCharacterBase.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/RPGAttributeSet.h"
#include "Component/CombatComponentBase.h"
#include "MotionWarpingComponent.h"

// Sets default values
ARPGCharacterBase::ARPGCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled =false;

	//@ 데칼 적용 false
	GetMesh()->bReceivesDecals = false;

	RPGAbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>(TEXT("RPGAbilitySystemComponent"));

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
	
}

UAbilitySystemComponent* ARPGCharacterBase::GetAbilitySystemComponent() const
{
	return GetRPGAbilitySystemComponent();
}

UCombatComponentBase* ARPGCharacterBase::GetCombatComponent() const
{
	return nullptr;
}

void ARPGCharacterBase::CreateDefaultAttributeSet()
{
	RPGAttributeSet = CreateDefaultSubobject<URPGAttributeSet>(TEXT("RPGAttributeSet"));
}

UUIComponentBase* ARPGCharacterBase::GetUIComponent() const
{
	return nullptr;
}

void ARPGCharacterBase::SetInteractTargetActor(AActor* TargetActor)
{
	InteractTargetActor = TargetActor;
}

FString ARPGCharacterBase::GetObjectiveName()
{
	return ObjectiveName;
}

void ARPGCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!IsValid(RPGAbilitySystemComponent)) return; 
	
	RPGAbilitySystemComponent->InitAbilityActorInfo(this, this);
}

