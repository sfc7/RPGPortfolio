// LJS

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InteractManager.generated.h"

class APlayerCharacterBase;
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
	void StartInteract(AActor* TargetActor, APlayerCharacterBase* Player);

	UFUNCTION(BlueprintCallable)
	void StopInteract(AActor* TargetActor, APlayerCharacterBase* Player);
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
};
