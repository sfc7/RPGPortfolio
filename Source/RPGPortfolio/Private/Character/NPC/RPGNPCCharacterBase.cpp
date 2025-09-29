// LJS


#include "Character/NPC/RPGNPCCharacterBase.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Widget/RPGWidgetBase.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "Component/Player/QuestNPCComponent.h"

ARPGNPCCharacterBase::ARPGNPCCharacterBase()
{
	InteractCollisionBox = CreateDefaultSubobject<UBoxComponent>("InteractCollisionBox");
	InteractCollisionBox->SetupAttachment(GetMesh());
	InteractCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InteractCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxBeginOverlap);
	InteractCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxEndOverlap);
	
	InteractWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("InteractWidgetComponent");
	InteractWidgetComponent->SetupAttachment(GetMesh());
	InteractWidgetComponent->SetVisibility(false);

	QuestNPCComponent = CreateDefaultSubobject<UQuestNPCComponent>("QuestNPCComponent");
}

void ARPGNPCCharacterBase::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacterBase* PC = Cast<APlayerCharacterBase>(OtherActor))
	{
		InteractWidgetComponent->SetVisibility(true);
	}
}

void ARPGNPCCharacterBase::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APlayerCharacterBase* PC = Cast<APlayerCharacterBase>(OtherActor))
	{
		InteractWidgetComponent->SetVisibility(false);
	}
}

void ARPGNPCCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	URPGWidgetBase* InteractWidget = Cast<URPGWidgetBase>(InteractWidgetComponent->GetUserWidgetObject());
	if (InteractWidget)
	{
		InteractWidget->InitMonsterCreatedWidget(this);
	}
}
