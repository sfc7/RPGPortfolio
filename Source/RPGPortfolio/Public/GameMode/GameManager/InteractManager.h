// LJS

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGStructTypes.h"
#include "InteractManager.generated.h"

class APlayerCharacterBase;
struct FInteractionData;
/**
 * UInteractManager
 * 
 * 상호작용 관리 서브시스템
 */
UCLASS()
class RPGPORTFOLIO_API UInteractManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UInteractManager();

	//@ 상호작용 시작
	UFUNCTION(BlueprintCallable)
	void StartInteract(AActor* TargetActor, APlayerCharacterBase* Player, EInteractType InteractType);

	//@ 상호작용 대상 설정
	UFUNCTION(BlueprintCallable)
	void SetInteractTarget(AActor* TargetActor, APlayerCharacterBase* Player, EInteractType InteractType);

	//@ 상호작용 대상 제거 
	UFUNCTION(BlueprintCallable)
	void RemoveInteractTarget(AActor* TargetActor, APlayerCharacterBase* Player, EInteractType InteractType);

	//@ 라인트레이스형 상호작용
	UFUNCTION(BlueprintCallable)
	void InteractTrace(APlayerCharacterBase* Player);

	//@ 상호작용 빈도 체크
	UFUNCTION(BlueprintCallable)
	bool CheckIneractionFrequency(FInteractionData InteractionTargetData);

	//@ 플레이어에게 상호작용 가능 GamePlayTag 적용
	UFUNCTION(BlueprintCallable)
	void ApplyCanInteractGamePlayTag(APlayerCharacterBase* PlayerCharacter);

	//@ 플레이어에게 상호작용 가능 GamePlayTag 제거
	UFUNCTION(BlueprintCallable)
	void RemoveCanInteractGamePlayTag(APlayerCharacterBase* PlayerCharacter);

	UFUNCTION(BlueprintCallable)
	void HandleStorageNPCInteraction(ARPGNPCCharacterBase* NPC, APlayerCharacterBase* Player);

	UFUNCTION(BlueprintCallable)
	void HandleConversationalNPCInteraction(ARPGNPCCharacterBase* NPC, APlayerCharacterBase* Player);

	UFUNCTION(BlueprintCallable)
	void HandleQuestNPCInteraction(ARPGNPCCharacterBase* NPC, APlayerCharacterBase* Player);

	UFUNCTION(BlueprintCallable)
	void HandleStoreNPCInteraction(ARPGNPCCharacterBase* NPC, APlayerCharacterBase* Player);

	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	//@ 상호작용 체크 빈도
	float InteractionCheckFrequency = 0.1f;
	
};
