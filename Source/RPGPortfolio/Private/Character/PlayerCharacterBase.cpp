// LJS


#include "Character/Player/PlayerCharacterBase.h"
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
#include "Component/Player/PlayerInventoryComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameMode/GameManager/InteractManager.h"

APlayerCharacterBase::APlayerCharacterBase()
{
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

	PlayerCombatComponent = CreateDefaultSubobject<UPlayerCombatComponent>(TEXT("PlayerCombatComponent"));
	PlayerUIComponent = CreateDefaultSubobject<UPlayerUIComponent>(TEXT("PlayerUIComponent"));
	PlayerItemInventoryComponent = CreateDefaultSubobject<UPlayerInventoryComponent>(TEXT("PlayerItemInventoryComponent"));
	PlayerPotionHotbar = CreateDefaultSubobject<UPlayerInventoryComponent>(TEXT("PlayerPotionHotbar"));

	ObjectPoolComponent = CreateDefaultSubobject<UObjectPoolComponent>(TEXT("ObjectPoolComponent"));
	
	GetCapsuleComponent()->OnComponentBeginOverlap.AddUniqueDynamic(this, &APlayerCharacterBase::OnCollisionBoxBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddUniqueDynamic(this, &APlayerCharacterBase::OnCollisionBoxEndOverlap);
	
	CreateDefaultAttributeSet();
}

void APlayerCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

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

UPlayerInventoryComponent* APlayerCharacterBase::GetPlayerPotionHotBar() const
{
	return PlayerPotionHotbar;
}

UObjectPoolComponent* APlayerCharacterBase::GetObjectPoolComponent() const
{
	return ObjectPoolComponent;
}

void APlayerCharacterBase::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnCollisionBoxBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (Cast<IUInteractionInterface>(OtherActor))
	{
		if (ARPGNPCCharacterBase* TargetNPC = Cast<ARPGNPCCharacterBase>(OtherActor))
		{
			InteractManager->ApplyCanInteractGamePlayTag(this);
			InteractManager->SetInteractTarget(TargetNPC, this, EInteractType::NPC);
		}
	}
}

void APlayerCharacterBase::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnCollisionBoxEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (Cast<IUInteractionInterface>(OtherActor))
	{
		if (ARPGNPCCharacterBase* TargetNPC = Cast<ARPGNPCCharacterBase>(OtherActor))
		{
			InteractManager->RemoveCanInteractGamePlayTag(this);
			InteractManager->RemoveInteractTarget(TargetNPC, this, EInteractType::NPC);
		}
	}
}

void APlayerCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
 
	if (!CharacterStartUpData.IsNull())
	{
		if (UDataAsset_AbilitySetBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			LoadedData->GiveAbilitySystemComponent(RPGAbilitySystemComponent);
		}
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
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	if (IsValid(Subsystem))
	{
		Subsystem->AddMappingContext(InputConfigDataAsset->DefaultInputMappingContext, 0);

		UPlayerEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UPlayerEnhancedInputComponent>(PlayerInputComponent);

		if (IsValid(PlayerEnhancedInputComponent))
		{
			PlayerEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, RPGGameplayTag::InputTag_Move_Keyboard, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
			PlayerEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, RPGGameplayTag::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
			PlayerEnhancedInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
			PlayerEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, RPGGameplayTag::InputTag_ShowDebug_Keyboard, ETriggerEvent::Triggered, this, &ThisClass::Input_ShowDebug);
			PlayerEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, RPGGameplayTag::InputTag_CallPauseMenu_Keyboard, ETriggerEvent::Triggered, this, &ThisClass::Input_CallPauseMenu);
			PlayerEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, RPGGameplayTag::InputTag_CallInventory_Keyboard, ETriggerEvent::Triggered, this, &ThisClass::Input_CallInventoryUI);
			PlayerEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, RPGGameplayTag::InputTag_CallQuestUI_Keyboard, ETriggerEvent::Triggered, this, &ThisClass::Input_CallQuestUI);
		}
	}
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
	if (PC)
	{
		PC->ConsoleCommand(TEXT("showdebug abilitysystem"), true);
	}
}

void APlayerCharacterBase::Input_CallPauseMenu()
{
	GetGameInstance()->GetSubsystem<UGeneralGameManager>()->GetUIManager()->ShowUIAsync(EUICategory::PauseMenuUI, GetWorld());
}

void APlayerCharacterBase::Input_CallInventoryUI()
{
	GetGameInstance()->GetSubsystem<UGeneralGameManager>()->GetUIManager()->ShowUIAsync(EUICategory::InventoryUI, GetWorld());
}

void APlayerCharacterBase::Input_CallQuestUI()
{
	GetGameInstance()->GetSubsystem<UGeneralGameManager>()->GetUIManager()->ShowUIAsync(EUICategory::QuestUI, GetWorld());
}

void APlayerCharacterBase::Input_Interact()
{
	GetGameInstance()->GetSubsystem<UGeneralGameManager>()->GetInteractManager()->InteractTrace(this);
}

