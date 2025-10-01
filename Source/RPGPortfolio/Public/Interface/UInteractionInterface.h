// LJS

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UInteractionInterface.generated.h"

//@ 상호작용 가능한 타입 열거형
UENUM()
enum class EInteractableType : uint8
{
	Active UMETA(DisplayName = "Active"),
	Hold UMETA(DisplayName = "Hold") ,
	NPCConversation UMETA(DisplayName = "NPCConversation"),
	Toggle UMETA(DisplayName = "Toggle"),
};

//@ 상호작용 가능한 데이터 구조체
USTRUCT()
struct FInteractableData
{
	GENERATED_USTRUCT_BODY()

	FInteractableData() :
		InteractableType(EInteractableType::Active),
		Name(FText::GetEmpty()),
		InteractionDuration(0.0f) {
	};

	//@ 상호작용 타입
	UPROPERTY(EditAnywhere)
	EInteractableType InteractableType;

	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(VisibleAnywhere)
	float InteractionDuration;
};

//@ 상호작용 인터페이스 UObject 래퍼 클래스 - 언리얼 엔진 인터페이스 시스템용
UINTERFACE(MinimalAPI)
class UUInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * IUInteractionInterface
 * 
 * 상호작용 가능한 오브젝트 인터페이스
 */
class RPGPORTFOLIO_API IUInteractionInterface
{
	GENERATED_BODY()
public:
	//@ 상호작용 가능한 데이터
	FInteractableData InteractableData;

	//@ 목표 이름 
	FString ObjectiveName;
	
	UFUNCTION()
	virtual void OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
