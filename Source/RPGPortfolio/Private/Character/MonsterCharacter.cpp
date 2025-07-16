// LJS


#include "Character/MonsterCharacter.h"

#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/Monster/MonsterCombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/AssetManager.h"
#include "NiagaraFunctionLibrary.h"
#include "Component/Monster/MonsterUIComponent.h"
#include "Components/WidgetComponent.h"
#include "DataAsset/DataAsset_AbilitySetBase.h"
#include "Widget/RPGWidgetBase.h"
#include "Components/BoxComponent.h"
#include "RPGFunc.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Quest/RPGQuestSystemActor.h"
#include "WorldStatic/DamageIndicator.h"
#include "Navigation/PathFollowingComponent.h"


AMonsterCharacter::AMonsterCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;

	MonsterCombatComponent = CreateDefaultSubobject<UMonsterCombatComponent>("MonsterCombatComponent");
	MonsterUIComponent = CreateDefaultSubobject<UMonsterUIComponent>("MonsterUIComponent");
	MonsterHpWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("MonsterHpWidgetComponent");
	MonsterHpWidgetComponent->SetupAttachment(GetMesh());
		
	CreateDefaultAttributeSet();

	LeftHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("LeftHandCollisionBox");
	LeftHandCollisionBox->SetupAttachment(GetMesh());
	LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxBeginOverlap);

	RightHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("RightHandCollisionBox");
	RightHandCollisionBox->SetupAttachment(GetMesh());
	RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxBeginOverlap);
}

void AMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();

	URPGWidgetBase* HpWidget = Cast<URPGWidgetBase>(MonsterHpWidgetComponent->GetUserWidgetObject());
	if (HpWidget)
	{
		HpWidget->InitMonsterCreatedWidget(this);
	}
}

UCombatComponentBase* AMonsterCharacter::GetCombatComponent() const
{
	return MonsterCombatComponent;
}

void AMonsterCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitEnemyStartUpData();
}

#if WITH_EDITOR
void AMonsterCharacter::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, LeftHandBoneName))
	{
		LeftHandCollisionBox->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftHandBoneName);
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, RightHandBoneName))
	{
		RightHandCollisionBox->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightHandBoneName);
	}
}
#endif

void AMonsterCharacter::MonsterDeath(TSoftObjectPtr<UNiagaraSystem> _DeathNiagaraEffectSoftObject)
{
	GetMesh()->bPauseAnims = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	
	MonsterUIComponent->RemoveMonsterDrawnWidget();
	
	UNiagaraSystem* DeathNiagara = _DeathNiagaraEffectSoftObject.LoadSynchronous();

	UNiagaraComponent* DeathEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
	DeathNiagara, GetMesh(), NAME_None, FVector::ZeroVector, FRotator::ZeroRotator,
	EAttachLocation::SnapToTarget, true, true, ENCPoolMethod::None, true);
	
	if (DeathEffect)
	{
		DeathEffect->OnSystemFinished.AddDynamic(this, &ThisClass::OnDeathEffectFinished);
	}

	if (APlayerCharacterBase* Player = Cast<APlayerCharacterBase>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		Player->OnInteractQuest.Broadcast(ObjectiveName);
	}
}

void AMonsterCharacter::OnDeathEffectFinished(UNiagaraComponent* FinishedComponent)
{
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
    
	for (AActor* AttachedActor : AttachedActors)
	{
		if (IsValid(AttachedActor))
		{
			AttachedActor->Destroy();
		}
	}
	
	Destroy();
}

UUIComponentBase* AMonsterCharacter::GetUIComponent() const
{
	return MonsterUIComponent;
}

UMonsterUIComponent* AMonsterCharacter::GetMonsterUIComponent() const
{
	return MonsterUIComponent;
}

void AMonsterCharacter::SpawnDamageIndicator(float Damage)
{
	FVector MonsterRightVector = GetActorRightVector();
	FVector MonsterUpVector = GetActorUpVector();

	FVector SocketLocation = GetMesh()->GetSocketLocation(FName(TEXT("DamageIndicatorSocket")));
	SocketLocation += (MonsterRightVector * 20.f) + (MonsterUpVector * 20.f);

	FActorSpawnParameters SpawnParams;

	ADamageIndicator* SpawnDamageIndicator = GetWorld()->SpawnActorDeferred<ADamageIndicator>(
			DamageIndicator,
			FTransform(FRotator::ZeroRotator, SocketLocation),
			nullptr,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
		);

	if (SpawnDamageIndicator)
	{
		SpawnDamageIndicator->SetDamage(Damage);
		UGameplayStatics::FinishSpawningActor(SpawnDamageIndicator, FTransform(FRotator::ZeroRotator, SocketLocation));
	}
}

void AMonsterCharacter::Teleport(FVector TargetLocation, AActor* TargetActor)
{
	AAIController* MonsterAIController = Cast<AAIController>(GetController());
	if (MonsterAIController)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		GetCharacterMovement()->MaxFlySpeed = 1500.f;
		GetCharacterMovement()->MaxAcceleration = 99999.f;
		GetMesh()->SetVisibility(false ,true);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn,ECollisionResponse::ECR_Ignore);


		if (IsValid(TeleportBody))
		{
			TeleportBodyComponent = UGameplayStatics::SpawnEmitterAttached(
				TeleportBody,
				GetMesh(),
				TEXT("spine_01"),
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				FVector::OneVector,
				EAttachLocation::KeepRelativeOffset,
				true,
				EPSCPoolMethod::None,
				true
			);
		}

		if (IsValid(TeleportTrail))
		{
			TeleportTrailComponent = UGameplayStatics::SpawnEmitterAttached(
				TeleportTrail,
				GetMesh(),
				TEXT("spine_01"),
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				FVector::OneVector,
				EAttachLocation::KeepRelativeOffset,
				true,
				EPSCPoolMethod::None,
				true
			);
		}
		MonsterAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &AMonsterCharacter::OnMoveCompleted);

		if (TargetActor)
		{
			MonsterAIController->MoveToActor(TargetActor, 100.f);
		}
		else
		{
			MonsterAIController->MoveToLocation(TargetLocation, 100.f);
		}

	}
}

void AMonsterCharacter::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
	}
    
	TeleportEnd();
}
void AMonsterCharacter::TeleportEnd()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetCharacterMovement()->MaxAcceleration = 2048.0;
	GetMesh()->SetVisibility(true ,true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn,ECollisionResponse::ECR_Block);

	if (TeleportBodyComponent)
	{
		TeleportBodyComponent->DestroyComponent();
	}
	if (TeleportTrailComponent)
	{
		TeleportTrailComponent->DestroyComponent();
	}
	
	OnTeleportEnd.Broadcast();
	
	FTimerHandle Waithandle;
	GetWorld()->GetTimerManager().SetTimer(Waithandle, FTimerDelegate::CreateLambda([&]() {
		if (TeleportBodyComponent) {
			TeleportBodyComponent->DestroyComponent();
		}
		if (TeleportTrailComponent) {
			TeleportTrailComponent->DestroyComponent();	
		}
	}), 0.5f, false);
}



void AMonsterCharacter::OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (URPGFunc::CheckTargetTeamAgent(this, HitPawn))
		{
			MonsterCombatComponent->OnHitTargetActor(HitPawn, 0, 0, EWeaponAttackType::None, EWeaponType::Body);
		}
	}
}

void AMonsterCharacter::InitEnemyStartUpData()
{
	if (CharacterStartUpData.IsNull())
	{
		return;
	}
	
	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		CharacterStartUpData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
		[this]()
				{
					if (UDataAsset_AbilitySetBase* LoadedData = CharacterStartUpData.Get())
					{
						LoadedData->GiveAbilitySystemComponent(RPGAbilitySystemComponent);
					}
				}
			)
		);
}
