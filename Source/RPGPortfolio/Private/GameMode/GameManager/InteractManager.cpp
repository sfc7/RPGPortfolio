// LJS


#include "GameMode/GameManager/InteractManager.h"

#include "Character/NPC/RPGNPCCharacterBase.h"

UInteractManager::UInteractManager()
{
}

void UInteractManager::StartInteract(AActor* TargetActor, APlayerCharacterBase* Player)
{
	if (ARPGNPCCharacterBase* NPC = Cast<ARPGNPCCharacterBase>(TargetActor))
	{
		NPC->SetAccessPlayerCharacter(Player);
		// Player->getcontroller()->accessactor(targetactor);
	}
}

void UInteractManager::StopInteract(AActor* TargetActor, APlayerCharacterBase* Player)
{
	if (ARPGNPCCharacterBase* NPC = Cast<ARPGNPCCharacterBase>(TargetActor))
	{
		NPC->SetAccessPlayerCharacter(nullptr);
		// Player->getcontroller()->accessactor(targetactor);
	}
}

void UInteractManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UInteractManager::Deinitialize()
{
	Super::Deinitialize();
}
