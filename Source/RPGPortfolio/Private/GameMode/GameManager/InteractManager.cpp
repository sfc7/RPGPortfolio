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
#include "Component/Player/PlayerInventoryComponent.h"
#include "Component/Player/QuestNPCComponent.h"

UInteractManager::UInteractManager()
{
}

void UInteractManager::StartInteract(AActor* TargetActor, APlayerCharacterBase* Player, EInteractType InteractType)
{
	if (!IsValid(TargetActor) || !IsValid(Player)) return;
	
	// 일단 NPC 상호작용만 처리
	const bool bIsNPCInteract = (InteractType == EInteractType::NPC);
	if (!bIsNPCInteract) return;

	ARPGNPCCharacterBase* const NPC = Cast<ARPGNPCCharacterBase>(TargetActor);
	if (!IsValid(NPC)) return;

	// NPC 타입에 따른 상호작용 처리
	const ENPCType NPCType = NPC->GetNPCType();
	
	switch (NPCType)
	{
	case ENPCType::Storage:
		HandleStorageNPCInteraction(NPC, Player);
		break;
		
	case ENPCType::Conversational:
		HandleConversationalNPCInteraction(NPC, Player);
		break;
		
	case ENPCType::Quest:
		HandleQuestNPCInteraction(NPC, Player);
		break;
		
	case ENPCType::Store:
		HandleStoreNPCInteraction(NPC, Player);
		break;
		
	default:
		break;
	}
}

void UInteractManager::SetInteractTarget(AActor* TargetActor, APlayerCharacterBase* Player, EInteractType InteractType)
{
	if (!IsValid(TargetActor) || !IsValid(Player)) return;

	// 일단 NPC 상호작용만 처리
	const bool bIsNPCInteract = (InteractType == EInteractType::NPC);
	if (!bIsNPCInteract) return;

	ARPGNPCCharacterBase* const NPC = Cast<ARPGNPCCharacterBase>(TargetActor);
	if (!IsValid(NPC)) return;

	// NPC와 플레이어 간 상호작용 설정
	NPC->SetAccessPlayerCharacter(Player);
	Player->SetInteractTargetActor(TargetActor);
}

void UInteractManager::RemoveInteractTarget(AActor* TargetActor, APlayerCharacterBase* Player, EInteractType InteractType)
{
	if (!IsValid(TargetActor) || !IsValid(Player)) return;

	// 일단 NPC 상호작용만 처리
	const bool bIsNPCInteract = (InteractType == EInteractType::NPC);
	if (!bIsNPCInteract) return;

	ARPGNPCCharacterBase* const NPC = Cast<ARPGNPCCharacterBase>(TargetActor);
	if (!IsValid(NPC)) return;

	// NPC와 플레이어 간 상호작용 제거
	NPC->SetAccessPlayerCharacter(nullptr);
	Player->SetInteractTargetActor(nullptr);
}

void UInteractManager::InteractTrace(APlayerCharacterBase* Player)
{
	if (!IsValid(Player)) return;

	UCameraComponent* const CameraComponent = Player->GetCameraComponent();
	if (!IsValid(CameraComponent)) return;

	// 상호작용 트레이스 설정
	const FVector CameraStartLocation = CameraComponent->GetComponentLocation();
	const FVector CameraForwardVector = CameraComponent->GetForwardVector();
	const float TraceDistance = 800.f;
	const FVector CameraEndLocation = CameraStartLocation + (CameraForwardVector * TraceDistance);
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Player);
	
	FHitResult HitResult;
	
	// 구체 트레이스 실행
	const bool bIsHit = UKismetSystemLibrary::SphereTraceSingle(
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

	if (bIsHit)
	{
		if (Cast<IUInteractionInterface>(HitResult.GetActor()))
		{
			if (ARPGNPCCharacterBase* TargetNPC = Cast<ARPGNPCCharacterBase>(HitResult.GetActor()))
			{
				//Todo 
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
	if (!IsValid(PlayerCharacter)) return;

	UAbilitySystemComponent* const ASC = PlayerCharacter->GetAbilitySystemComponent();
	if (!IsValid(ASC)) return;

	// 이미 태그가 있는지 확인 후 추가
	const bool bAlreadyHasTag = ASC->HasMatchingGameplayTag(RPGGameplayTag::Player_Status_CanInteract);
	if (!bAlreadyHasTag)
	{
		ASC->AddLooseGameplayTag(RPGGameplayTag::Player_Status_CanInteract);
	}
}

void UInteractManager::RemoveCanInteractGamePlayTag(APlayerCharacterBase* PlayerCharacter)
{
	if (!IsValid(PlayerCharacter)) return;
	
	UAbilitySystemComponent* const ASC = PlayerCharacter->GetAbilitySystemComponent();
	if (!IsValid(ASC)) return;

	// 태그가 있는지 확인 후 제거
	const bool bHasTag = ASC->HasMatchingGameplayTag(RPGGameplayTag::Player_Status_CanInteract);
	if (bHasTag)
	{
		ASC->RemoveLooseGameplayTag(RPGGameplayTag::Player_Status_CanInteract);
	}
}

void UInteractManager::HandleStorageNPCInteraction(ARPGNPCCharacterBase* NPC, APlayerCharacterBase* Player)
{
	ANPC_Storage* const StorageNPC = Cast<ANPC_Storage>(NPC);
	if (!IsValid(StorageNPC)) return;
	
	UUserWidget* const StorageWidget = StorageNPC->GetStorageWidget();
	if (!IsValid(StorageWidget)) return;

	// 창고에 플레이어 컨트롤러 설정 및 플레이어 인벤토리 전략 변경
	APlayerController* const PlayerController = Player->GetController<APlayerController>();
	if (IsValid(PlayerController))
	{
		StorageWidget->SetOwningPlayer(PlayerController);
		Player->GetPlayerInventoryComponent()->SetCurrentInventorySituationStrategy(EInventorySituationStrategy::InOpenStorage);
	}
	
	// 위젯을 뷰포트에 추가
	StorageWidget->AddToViewport();
}

void UInteractManager::HandleConversationalNPCInteraction(ARPGNPCCharacterBase* NPC, APlayerCharacterBase* Player)
{
	ANPC_HumanNPC* const HumanNPC = Cast<ANPC_HumanNPC>(NPC);
	if (!IsValid(HumanNPC)) return;
	
	const FString ObjectiveName = HumanNPC->GetObjectiveName();

	Player->OnInteractQuest.Broadcast(ObjectiveName);
	
	// 대화 표시
	UQuestNPCComponent* const QuestNPCComponent = NPC->GetQuestNPCComponent();
	if (IsValid(QuestNPCComponent))
	{
		QuestNPCComponent->DisplayConversation();
	}
}

void UInteractManager::HandleQuestNPCInteraction(ARPGNPCCharacterBase* NPC, APlayerCharacterBase* Player)
{
	ANPC_HumanNPC* const HumanNPC = Cast<ANPC_HumanNPC>(NPC);
	if (!IsValid(HumanNPC)) return;
	
	const FString ObjectiveName = HumanNPC->GetObjectiveName();

	Player->OnInteractQuest.Broadcast(ObjectiveName);
	
	// 퀘스트 상호작용 실행
	UQuestNPCComponent* const QuestNPCComponent = NPC->GetQuestNPCComponent();
	if (IsValid(QuestNPCComponent))
	{
		QuestNPCComponent->InteractWith();
	}
}

void UInteractManager::HandleStoreNPCInteraction(ARPGNPCCharacterBase* NPC, APlayerCharacterBase* Player)
{
	ANPC_HumanNPC* const HumanNPC = Cast<ANPC_HumanNPC>(NPC);
	if (!IsValid(HumanNPC)) return;

	UUserWidget* const StoreWidget = HumanNPC->GetStoreWidget();
	if (!IsValid(StoreWidget)) return;

	// 창고에 플레이어 컨트롤러 설정 및 플레이어 인벤토리 전략 변경
	APlayerController* const PlayerController = Player->GetController<APlayerController>();
	if (IsValid(PlayerController))
	{
		StoreWidget->SetOwningPlayer(PlayerController);
		Player->GetPlayerInventoryComponent()->SetCurrentInventorySituationStrategy(EInventorySituationStrategy::InOpenStore);
	}
	
	// 위젯을 뷰포트에 추가
	StoreWidget->AddToViewport();
}


void UInteractManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UInteractManager::Deinitialize()
{
	Super::Deinitialize();
}
