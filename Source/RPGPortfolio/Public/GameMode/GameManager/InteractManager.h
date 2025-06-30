// LJS

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGStructTypes.h"
#include "InteractManager.generated.h"

class APlayerCharacterBase;
struct FInteractionData;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UInteractManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UInteractManager();

	UFUNCTION(BlueprintCallable)
	void StartInteract(AActor* TargetActor, APlayerCharacterBase* Player, EInteractType InteractType);

	UFUNCTION(BlueprintCallable)
	void SetInteractTarget(AActor* TargetActor, APlayerCharacterBase* Player, EInteractType InteractType);

	UFUNCTION(BlueprintCallable)
	void RemoveInteractTarget(AActor* TargetActor, APlayerCharacterBase* Player, EInteractType InteractType);
	
	UFUNCTION(BlueprintCallable)
	void InteractTrace(APlayerCharacterBase* Player);

	UFUNCTION(BlueprintCallable)
	bool CheckIneractionFrequency(FInteractionData InteractionTargetData);

	UFUNCTION(BlueprintCallable)
	void ApplyCanInteractGamePlayTag(APlayerCharacterBase* PlayerCharacter);

	UFUNCTION(BlueprintCallable)
	void RemoveCanInteractGamePlayTag(APlayerCharacterBase* PlayerCharacter);
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	float InteractionCheckFrequency = 0.1f;
	
};
