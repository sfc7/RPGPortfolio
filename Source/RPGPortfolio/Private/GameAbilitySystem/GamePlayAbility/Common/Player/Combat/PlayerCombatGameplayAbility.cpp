// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/PlayerCombatGameplayAbility.h"

#include "Character/MonsterCharacter.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "GameAbilitySystem/GameplayTask/Player/RPGAT_Player_RotateTarget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerCombatGameplayAbility::UPlayerCombatGameplayAbility()
{
}

bool UPlayerCombatGameplayAbility::FindNearestEnemyBeforeAttack(float BoxExtent)
{
	TArray<FHitResult> HitResults;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetPlayerCharacterFromActorInfo(),
		GetPlayerCharacterFromActorInfo()->GetActorLocation(),
		GetPlayerCharacterFromActorInfo()->GetActorLocation(),
		FVector(BoxExtent, BoxExtent, BoxExtent) / 2,
		GetPlayerCharacterFromActorInfo()->GetActorRotation(),
		ObjectTypes,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::None,
		HitResults,
		true
	);

	TArray<AActor*> FindEnemies;
	for (const FHitResult& HitResult : HitResults)
	{
		if (AMonsterCharacter* HitMonster =  Cast<AMonsterCharacter>(HitResult.GetActor()))
		{
			FindEnemies.AddUnique(HitMonster);
		}
	}

	if (FindEnemies.IsEmpty())
	{
		return false;
	}
	else
	{
		float ClosetDistance = 0.f;
		RotateTargetActor = UGameplayStatics::FindNearestActor(GetPlayerCharacterFromActorInfo()->GetActorLocation(), FindEnemies, ClosetDistance);
		FindRototation = UKismetMathLibrary::FindLookAtRotation(
				GetPlayerCharacterFromActorInfo()->GetActorLocation(),
				RotateTargetActor->GetActorLocation()
				);
	}

	return true;
}

void UPlayerCombatGameplayAbility::RotateTargetTickBeforeAttack(float DeltaTime)
{
	if (HasMatchingGameplayTag(RPGGameplayTag::Character_Status_Dead)) return;
	if (!RotateTargetActor) return;

	const FRotator TargetRot = FMath::RInterpTo(GetPlayerCharacterFromActorInfo()->GetActorRotation(),FindRototation,DeltaTime,15.0f);

	GetPlayerCharacterFromActorInfo()->SetActorRotation(FRotator(0.f,TargetRot.Yaw,0.f));
}

void UPlayerCombatGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	APlayerCharacterBase* OwnerCharacter = Cast<APlayerCharacterBase>(ActorInfo->AvatarActor.Get());
	if (!OwnerCharacter)
	{
		return;
	}

	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void UPlayerCombatGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	APlayerCharacterBase* OwnerCharacter = Cast<APlayerCharacterBase>(ActorInfo->AvatarActor.Get());
	if (!OwnerCharacter)
	{
		return;
	}

	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}
