// LJS


#include "GameMode/GameManager/InteractManager.h"

#include "Blueprint/UserWidget.h"
#include "Character/NPC/NPC_Storage.h"
#include "Character/NPC/RPGNPCCharacterBase.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "GameMode/GameManager/UIManager.h"
#include "Camera/CameraComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interface/UInteractionInterface.h"
#include "AbilitySystemComponent.h"
#include "Character/NPC/NPC_HumanNPC.h"
#include "Component/Player/QuestNPCComponent.h"

UInteractManager::UInteractManager()
{
}

void UInteractManager::StartInteract(AActor* TargetActor, APlayerCharacterBase* Player, EInteractType InteractType)
{
	if (InteractType == EInteractType::NPC)
	{
		if (ARPGNPCCharacterBase* NPC = Cast<ARPGNPCCharacterBase>(TargetActor))
		{
			if (NPC->GetNPCType() == ENPCType::Storage)
			{
				if (ANPC_Storage* Storage = Cast<ANPC_Storage>(NPC))
				{
					UUserWidget* StorageWidget = Storage->GetStorageWidget();

					if (APlayerController* PlayerController = Player->GetController<APlayerController>())
					{
						StorageWidget->SetOwningPlayer(PlayerController);
					} 
					StorageWidget->AddToViewport();
				}
			}
			else if (NPC->GetNPCType() == ENPCType::Conversational)
			{
				if (ANPC_HumanNPC* HumanNPC = Cast<ANPC_HumanNPC>(NPC))
				{
					FString ObjectiveName = HumanNPC->GetObjectiveName();
					
					Player->OnInteractQuest.Broadcast(ObjectiveName);
					NPC->GetQuestNPCComponent()->InteractWith();
				}
			}
			else if (NPC->GetNPCType() == ENPCType::Store)
			{
				if (ANPC_HumanNPC* HumanNPC = Cast<ANPC_HumanNPC>(NPC))
				{
					UUserWidget* StoreWidget = HumanNPC->GetStoreWidget();

					StoreWidget->AddToViewport();
				}
			
			}
		}
	}
}

void UInteractManager::SetInteractTarget(AActor* TargetActor, APlayerCharacterBase* Player, EInteractType InteractType)
{
	if (InteractType == EInteractType::NPC)
	{
		if (ARPGNPCCharacterBase* NPC = Cast<ARPGNPCCharacterBase>(TargetActor))
		{
			NPC->SetAccessPlayerCharacter(Player);
			Player->SetInteractTargetActor(TargetActor);
		}
	}
}

void UInteractManager::RemoveInteractTarget(AActor* TargetActor, APlayerCharacterBase* Player, EInteractType InteractType)
{
	if (InteractType == EInteractType::NPC)
	{
		if (ARPGNPCCharacterBase* NPC = Cast<ARPGNPCCharacterBase>(TargetActor))
		{
			NPC->SetAccessPlayerCharacter(nullptr);
			Player->SetInteractTargetActor(nullptr);
		}
	}
}

void UInteractManager::InteractTrace(APlayerCharacterBase* Player)
{
	FVector CameraStartLocation = Player->GetCameraComponent()->GetComponentLocation();
	FVector CameraEndLocation = CameraStartLocation + (Player->GetCameraComponent()->GetForwardVector() * 800.f);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Player);
	FHitResult HitResult;
	
	bool IsHit = UKismetSystemLibrary::SphereTraceSingle(
	this,
	CameraStartLocation,
	CameraEndLocation,
	10.0f,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Pawn),
	false,
	ActorsToIgnore,
	EDrawDebugTrace::Type::None,
	HitResult,
	true
	);

	if (IsHit)
	{
		if (Cast<IUInteractionInterface>(HitResult.GetActor()))
		{
			if (ARPGNPCCharacterBase* TargetNPC = Cast<ARPGNPCCharacterBase>(HitResult.GetActor()))
			{
				
			}
		}
	}
}

bool UInteractManager::CheckIneractionFrequency(FInteractionData InteractionTargetData)
{
	return (GetWorld()->TimeSince(InteractionTargetData.LastInteractionCheckTime) > InteractionCheckFrequency) ? true : false;
}

void UInteractManager::ApplyCanInteractGamePlayTag(APlayerCharacterBase* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		return;
	}

	if (UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent())
	{
		if (!ASC->HasMatchingGameplayTag(RPGGameplayTag::Player_Status_CanInteract))
		{
			ASC->AddLooseGameplayTag(RPGGameplayTag::Player_Status_CanInteract);
		}
	}
}

void UInteractManager::RemoveCanInteractGamePlayTag(APlayerCharacterBase* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		return;
	}

	if (UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent())
	{
		if (ASC->HasMatchingGameplayTag(RPGGameplayTag::Player_Status_CanInteract))
		{
			ASC->RemoveLooseGameplayTag(RPGGameplayTag::Player_Status_CanInteract);
		}
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
