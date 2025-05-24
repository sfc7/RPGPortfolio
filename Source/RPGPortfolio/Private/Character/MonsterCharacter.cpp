// LJS


#include "Character/MonsterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/Monster/MonsterCombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/AssetManager.h"
#include "NiagaraFunctionLibrary.h"
#include "DataAsset/DataAsset_AbilitySetBase.h"

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

	MonsterCombatComponent = CreateDefaultSubobject<UMonsterCombatComponent>(" MonsterCombatComponent");

	CreateDefaultAttributeSet();
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

void AMonsterCharacter::MonsterDeath(TSoftObjectPtr<UNiagaraSystem> _DeathNiagaraEffectSoftObject)
{
	GetMesh()->bPauseAnims = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UNiagaraSystem* DeathNiagara = _DeathNiagaraEffectSoftObject.LoadSynchronous();

	UNiagaraFunctionLibrary::SpawnSystemAttached(DeathNiagara, GetMesh(), NAME_None,FVector::ZeroVector,  FRotator::ZeroRotator,
		EAttachLocation::KeepWorldPosition, true, true, ENCPoolMethod::None, true);
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
