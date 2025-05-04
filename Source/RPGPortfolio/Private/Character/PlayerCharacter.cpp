// LJS


#include "Character/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Component/Player/PlayerEnhancedInputComponent.h"
#include "DataAsset/DataAsset_InputConfig.h"
#include "DataAsset/DataAsset_AbilitySetBase.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/Player/PlayerCombatComponent.h"

APlayerCharacter::APlayerCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 600.f, 0.f);
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
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
 
	if (!CharacterStartUpData.IsNull())
	{
		if (UDataAsset_AbilitySetBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			LoadedData->GiveAbilitySystemComponent(RPGAbilitySystemComponent);
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)	
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
		}
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
void APlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
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

void APlayerCharacter::Input_Look(const FInputActionValue& InputActionValue)
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

void APlayerCharacter::Input_AbilityInputPressed(FGameplayTag _InputTag)
{
	RPGAbilitySystemComponent->OnAbilityInputPressed(_InputTag);
}

void APlayerCharacter::Input_AbilityInputReleased(FGameplayTag _InputTag)
{
	RPGAbilitySystemComponent->OnAbilityInputReleased(_InputTag);
}
