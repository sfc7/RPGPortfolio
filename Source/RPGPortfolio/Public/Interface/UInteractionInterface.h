// LJS

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UInteractionInterface.generated.h"

UENUM()
enum class EInteractableType : uint8
{
	Active UMETA(DisplayName = "Active"),
	Hold UMETA(DisplayName = "Hold") ,
	NPCConversation UMETA(DisplayName = "NPCConversation"),
	Toggle UMETA(DisplayName = "Toggle"),
};

USTRUCT()
struct FInteractableData
{
	GENERATED_USTRUCT_BODY()

	FInteractableData() :
		InteractableType(EInteractableType::Active),
		Name(FText::GetEmpty()),
		InteractionDuration(0.0f) {
	};

	UPROPERTY(EditAnywhere)
	EInteractableType InteractableType;

	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(VisibleAnywhere)
	float InteractionDuration;
};

UINTERFACE(MinimalAPI)
class UUInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPGPORTFOLIO_API IUInteractionInterface
{
	GENERATED_BODY()
public:
	FInteractableData InteractableData;

	UFUNCTION()
	virtual void OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
