// LJS


#include "Character/Player/PlayerCharacterBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/NPC/RPGNPCCharacterBase.h"
#include "Component/Player/ObjectPoolComponent.h"
#include "Component/Player/PlayerEnhancedInputComponent.h"
#include "DataAsset/DataAsset_InputConfig.h"
#include "DataAsset/DataAsset_AbilitySetBase.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerAttributeSet.h"
#include "Component/Player/PlayerCombatComponent.h"
#include "Component/Player/PlayerUIComponent.h"
#include "DataAsset/DataAsset_RPGUIData.h"
#include "GameMode/GameManager/GeneralGameManager.h"
#include "GameMode/GameManager/UIManager.h"
#include "Component/InventoryComponent.h"
#include "Component/Player/PlayerSkillComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameMode/RPGGameInstance.h"
#include "GameMode/GameManager/InteractManager.h"
#include "Component/Player/PlayerEquipmentComponent.h"
#include "Component/Player/PlayerInventoryComponent.h"

APlayerCharacterBase::APlayerCharacterBase()
{
	// 기본 컴포넌트
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->BrakingDecelerationWalking= 2000.f;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 200.f;
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = true;

	// 커스텀 컴포넌트
	PlayerCombatComponent = CreateDefaultSubobject<UPlayerCombatComponent>(TEXT("PlayerCombatComponent"));
	PlayerUIComponent = CreateDefaultSubobject<UPlayerUIComponent>(TEXT("PlayerUIComponent"));
	PlayerItemInventoryComponent = CreateDefaultSubobject<UPlayerInventoryComponent>(TEXT("PlayerItemInventoryComponent"));
	PlayerPotionHotbar = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerPotionHotbar"));
	PlayerEquipmentComponent = CreateDefaultSubobject<UPlayerEquipmentComponent>(TEXT("PlayerEquipmentComponent"));
	ObjectPoolComponent = CreateDefaultSubobject<UObjectPoolComponent>(TEXT("ObjectPoolComponent"));
	PlayerSkillComponent = CreateDefaultSubobject<UPlayerSkillComponent>(TEXT("PlayerSkillComponent"));
	
	GetCapsuleComponent()->OnComponentBeginOverlap.AddUniqueDynamic(this, &APlayerCharacterBase::OnCollisionBoxBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddUniqueDynamic(this, &APlayerCharacterBase::OnCollisionBoxEndOverlap);

	// ASC
	CreateDefaultAttributeSet();
}

void APlayerCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// 상호작용 체크 빈도를 확인하고 상호작용 검사 수행
	if (InteractManager->CheckIneractionFrequency(InteractionTargetData))
	{
		PerformInteractionCheck();
	}
}

UCombatComponentBase* APlayerCharacterBase::GetCombatComponent() const
{
	return PlayerCombatComponent;
}

UUIComponentBase* APlayerCharacterBase::GetUIComponent() const
{
	return PlayerUIComponent;
}

UCameraComponent* APlayerCharacterBase::GetCameraComponent() const
{
	return CameraComponent;
}

UPlayerUIComponent* APlayerCharacterBase::GetPlayerUIComponent() const
{
	return PlayerUIComponent;
}

UPlayerInventoryComponent* APlayerCharacterBase::GetPlayerInventoryComponent() const
{
	return PlayerItemInventoryComponent;
}

UInventoryComponent* APlayerCharacterBase::GetPlayerPotionHotBar() const
{
	return PlayerPotionHotbar;
}

UObjectPoolComponent* APlayerCharacterBase::GetObjectPoolComponent() const
{
	return ObjectPoolComponent;
}

UPlayerSkillComponent* APlayerCharacterBase::GetPlayerSkillComponent() const
{
	return PlayerSkillComponent;
}

UPlayerEquipmentComponent* APlayerCharacterBase::GetPlayerEquipmentComponent() const
{
	return PlayerEquipmentComponent;
}

void APlayerCharacterBase::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnCollisionBoxBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// 대상이 상호작용 인터페이스 상속했는지 확인
	if (Cast<IUInteractionInterface>(OtherActor))
	{
		if (ARPGNPCCharacterBase* TargetNPC = Cast<ARPGNPCCharacterBase>(OtherActor))
		{
			// 상호작용 가능 게임플레이 태그 적용
			InteractManager->ApplyCanInteractGamePlayTag(this);
			// 상호작용 대상 설정
			InteractManager->SetInteractTarget(TargetNPC, this, EInteractType::NPC);
		}
	}
}

void APlayerCharacterBase::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnCollisionBoxEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	// 대상이 상호작용 인터페이스 상속했는지 확인
	if (Cast<IUInteractionInterface>(OtherActor))
	{
		if (ARPGNPCCharacterBase* TargetNPC = Cast<ARPGNPCCharacterBase>(OtherActor))
		{
			// 상호작용 가능 게임플레이 태그 제거
			InteractManager->RemoveCanInteractGamePlayTag(this);
			// 상호작용 대상 제거
			InteractManager->RemoveInteractTarget(TargetNPC, this, EInteractType::NPC);
		}
	}
}

void APlayerCharacterBase::SaveAllPlayerData(URPGSaveGame* SaveGame)
{
	if (!SaveGame) return;
    
	URPGAbilitySystemComponent* ASC = GetRPGAbilitySystemComponent();
	if (!IsValid(ASC)) return;

	UPlayerSkillComponent* SkillComp = GetComponentByClass<UPlayerSkillComponent>();
	if (!IsValid(SkillComp)) return;

	// 동적 어빌리티 속성 부여한 어빌리티들 세이브
	ASC->SaveDynamicAbilitiesToSaveGame(SaveGame);
	// 스킬 퀵슬롯 세이브
	SkillComp->SaveSkillQuickSlotsToSaveGame(SaveGame);	
}

void APlayerCharacterBase::LoadAllPlayerData(URPGSaveGame* SaveGame)
{
	if (!SaveGame) return;

	URPGAbilitySystemComponent* ASC = GetRPGAbilitySystemComponent();
	if (!IsValid(ASC)) return;

	UPlayerSkillComponent* SkillComp = GetComponentByClass<UPlayerSkillComponent>();
	if (!IsValid(SkillComp)) return;

	// 동적 어빌리티 속성 부여한 어빌리티들 로드
	ASC->LoadDynamicAbilitiesFromSaveGame(SaveGame);
	// 스킬 퀵슬롯 로드
	SkillComp->LoadSkillQuickSlotsFromSaveGame(SaveGame);
}

void APlayerCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	URPGGameInstance* RPGGameInstance = Cast<URPGGameInstance>(GetGameInstance());
	if (!IsValid(RPGGameInstance)) return;

	if (!CharacterStartUpData.IsNull())
	{
		if (UDataAsset_AbilitySetBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			LoadedData->GiveAbilitySystemComponent(RPGAbilitySystemComponent);
		}
	}
	
	if (IsValid(PlayerSkillComponent))
	{
		PlayerSkillComponent->GiveAbilitySystemComponent(RPGAbilitySystemComponent);
	}
	
	if (IsValid(PlayerUIComponent))
	{
		PlayerUIComponent->OnInitPlayerUIbyClassDelegate.Broadcast(PlayerCharacterClass);
	}

	InteractManager = GetGameInstance()->GetSubsystem<UInteractManager>();	
}

auto APlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) -> void
{
	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
	if (!IsValid(LocalPlayer)) return;
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	if (!IsValid(Subsystem)) return;

	UPlayerEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UPlayerEnhancedInputComponent>(PlayerInputComponent);
	if (!IsValid(PlayerEnhancedInputComponent)) return;
	
	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultInputMappingContext, 0);

	PlayerEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, RPGGameplayTag::InputTag_Move_Keyboard, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	PlayerEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, RPGGameplayTag::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
	PlayerEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, RPGGameplayTag::InputTag_SwitchTarget, ETriggerEvent::Triggered, this, &ThisClass::Input_SwitchTargetTriggered);
	PlayerEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, RPGGameplayTag::InputTag_SwitchTarget, ETriggerEvent::Completed, this, &ThisClass::Input_SwitchTargetCompleted);
	PlayerEnhancedInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
	PlayerEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, RPGGameplayTag::InputTag_ShowDebug_Keyboard, ETriggerEvent::Started, this, &ThisClass::Input_ShowDebug);
	PlayerEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, RPGGameplayTag::InputTag_CallPauseMenu_Keyboard, ETriggerEvent::Started, this, &ThisClass::Input_CallPauseMenu);
	PlayerEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, RPGGameplayTag::InputTag_CallInventory_Keyboard, ETriggerEvent::Started, this, &ThisClass::Input_CallInventoryUI);
	PlayerEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, RPGGameplayTag::InputTag_CallQuestUI_Keyboard, ETriggerEvent::Started, this, &ThisClass::Input_CallQuestUI);
	PlayerEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, RPGGameplayTag::InputTag_CallSkillUI_Keyboard, ETriggerEvent::Started, this, &ThisClass::Input_CallSkillUI);
}

void APlayerCharacterBase::CreateDefaultAttributeSet()
{
	RPGAttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("PlayerAttributeSet"));
}

void APlayerCharacterBase::BeginPlay()
{	
	Super::BeginPlay();
}

void APlayerCharacterBase::PerformInteractionCheck()
{
	InteractionTargetData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	InteractManager->InteractTrace(this);
}

void APlayerCharacterBase::Input_Move(const FInputActionValue& InputActionValue)
{	
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
 
	const FRotator MovementRotation(0.f,Controller->GetControlRotation().Yaw,0.f);
 
	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
 
		AddMovementInput(ForwardDirection,MovementVector.Y);
	}
 
	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
 
		AddMovementInput(RightDirection,MovementVector.X);
	}
}

void APlayerCharacterBase::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
 	
	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);	
	}
 
	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacterBase::Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue)
{
	SwitchDirection = InputActionValue.Get<FVector2D>();
}

void APlayerCharacterBase::Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue)
{
	FGameplayEventData EventData;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		SwitchDirection.X>0.f? RPGGameplayTag::Player_Event_SwitchTarget_Right : RPGGameplayTag::Player_Event_SwitchTarget_Left,
		EventData
		);
}

void APlayerCharacterBase::Input_AbilityInputPressed(FGameplayTag _InputTag)
{
	RPGAbilitySystemComponent->OnAbilityInputPressed(_InputTag);
}

void APlayerCharacterBase::Input_AbilityInputReleased(FGameplayTag _InputTag)
{
	RPGAbilitySystemComponent->OnAbilityInputReleased(_InputTag);
}

void APlayerCharacterBase::Input_ShowDebug()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!IsValid(PC)) return;

	// GAS용 콘솔 커맨드 
	PC->ConsoleCommand(TEXT("showdebug abilitysystem"), true);
}

void APlayerCharacterBase::Input_CallPauseMenu()
{
	GetGameInstance()->GetSubsystem<UGeneralGameManager>()->GetUIManager()->ShowUIAsync(EUICategory::PauseMenuUI, GetWorld());
}

void APlayerCharacterBase::Input_CallInventoryUI()
{
	GetGameInstance()->GetSubsystem<UGeneralGameManager>()->GetUIManager()->ShowUIAsync(EUICategory::InventoryUI, GetWorld());
	
	GetPlayerInventoryComponent()->SetCurrentInventorySituationStrategy(EInventorySituationStrategy::InOpenEquipment);
}

void APlayerCharacterBase::Input_CallQuestUI()
{
	GetGameInstance()->GetSubsystem<UGeneralGameManager>()->GetUIManager()->ShowUIAsync(EUICategory::QuestUI, GetWorld());
}

void APlayerCharacterBase::Input_CallSkillUI()
{
	GetGameInstance()->GetSubsystem<UGeneralGameManager>()->GetUIManager()->ShowUIAsync(EUICategory::SkillUI, GetWorld());
}

void APlayerCharacterBase::Input_Interact()
{
	GetGameInstance()->GetSubsystem<UGeneralGameManager>()->GetInteractManager()->InteractTrace(this);
}

