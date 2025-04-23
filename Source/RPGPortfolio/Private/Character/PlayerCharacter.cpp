// LJS


#include "Character/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Component/PlayerEnhancedInputComponent.h"
#include "DataAsset/DataAsset_InputConfig.h"
#include "GamePlayAbility/RPGGamePlayTag.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerCharacter::APlayerCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 600.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking= 2000.f;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 200.f;
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = true;


}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
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
			PlayerEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, RPGGameplayTag::InputTag_Move_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
			PlayerEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, RPGGameplayTag::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
		}
	}

	
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
