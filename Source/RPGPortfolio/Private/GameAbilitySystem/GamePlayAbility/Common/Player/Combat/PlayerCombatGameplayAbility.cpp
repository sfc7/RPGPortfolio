// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/PlayerCombatGameplayAbility.h"

#include "Character/MonsterCharacter.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "GameAbilitySystem/GameplayTask/RPGAT_ExecuteTaskOnTick.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UPlayerCombatGameplayAbility::UPlayerCombatGameplayAbility()
{
}

void UPlayerCombatGameplayAbility::FindNearestEnemyBeforeAttack(float BoxExtent)
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
		EDrawDebugTrace::Persistent,
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
		return;
	}
	else
	{
		float ClosetDistance = 0.f;
		RotateTargetActor = UGameplayStatics::FindNearestActor(GetPlayerCharacterFromActorInfo()->GetActorLocation(), FindEnemies, ClosetDistance);

		URPGAT_ExecuteTaskOnTick* TickTask = URPGAT_ExecuteTaskOnTick::ExecuteTaskOnTick(this);
		TickTask->OnAbilityTaskTick.AddDynamic(this, &UPlayerCombatGameplayAbility::RotateTargetTickBeforeAttack);
		TickTask->ReadyForActivation();
	}
	
}

void UPlayerCombatGameplayAbility::RotateTargetTickBeforeAttack(float DeltaTime)
{
	if (HasMatchingGameplayTag(RPGGameplayTag::Character_Status_Dead)) return;

	const FRotator FindRototation = UKismetMathLibrary::FindLookAtRotation(
			GetPlayerCharacterFromActorInfo()->GetActorLocation(),
			RotateTargetActor->GetActorLocation()
		);

	const FRotator TargetRot = FMath::RInterpTo(GetPlayerCharacterFromActorInfo()->GetActorRotation(),FindRototation,DeltaTime,5.0f);

	GetPlayerCharacterFromActorInfo()->SetActorRotation(FRotator(0.f,TargetRot.Yaw,0.f));
}
