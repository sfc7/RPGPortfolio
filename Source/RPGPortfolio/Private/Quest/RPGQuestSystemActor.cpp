// LJS


#include "Quest/RPGQuestSystemActor.h"

// Sets default values
ARPGQuestSystemActor::ARPGQuestSystemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ARPGQuestSystemActor::SetQuestID(FName QuestIDtoSet)
{
	QuestID = QuestIDtoSet;
}

// Called when the game starts or when spawned
void ARPGQuestSystemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARPGQuestSystemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

