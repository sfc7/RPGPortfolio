// LJS


#include "Character/MonsterCharacter.h"
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

	MonsterUIComponent->RemoveMonsterDrawnWidget();
	
	UNiagaraSystem* DeathNiagara = _DeathNiagaraEffectSoftObject.LoadSynchronous();

	UNiagaraComponent* DeathEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
	DeathNiagara, GetMesh(), NAME_None, FVector::ZeroVector, FRotator::ZeroRotator,
	EAttachLocation::SnapToTarget, true, true, ENCPoolMethod::None, true);
	
	if (DeathEffect)
	{
		DeathEffect->OnSystemFinished.AddDynamic(this, &ThisClass::OnDeathEffectFinished);
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

void AMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();

	URPGWidgetBase* HpWidget = Cast<URPGWidgetBase>(MonsterHpWidgetComponent->GetUserWidgetObject());
	if (HpWidget)
	{
		HpWidget->InitMonsterCreatedWidget(this);
	}
}

void AMonsterCharacter::OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (URPGFunc::CheckTargetTeamAgent(this, HitPawn))
		{
			MonsterCombatComponent->OnHitTargetActor(HitPawn);
			
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
