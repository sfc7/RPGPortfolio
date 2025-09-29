// LJS

#pragma once

#include "CoreMinimal.h"
#include "Character/RPGCharacterBase.h"
#include "RPGStructTypes.h"
#include "RPGNPCCharacterBase.generated.h"

class APlayerCharacterBase;
class UBoxComponent;
class UWidgetComponent;
class UQuestNPCComponent;

/**
 * ARPGNPCCharacterBase
 * 
 * NPC Character의 Base 클래스 
 */
UCLASS()
class RPGPORTFOLIO_API ARPGNPCCharacterBase : public ARPGCharacterBase
{
	GENERATED_BODY()
public:
	ARPGNPCCharacterBase();

	//@ 상호작용 당할 때 플레이어 캐싱
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	APlayerCharacterBase* AccessPlayerCharacter;

	//@ 상호작용용 충돌 박스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* InteractCollisionBox;
	
	UFUNCTION(BlueprintCallable)
	void SetAccessPlayerCharacter(APlayerCharacterBase* PlayerCharacter) { AccessPlayerCharacter = PlayerCharacter; }

	UFUNCTION(BlueprintCallable)
	APlayerCharacterBase* GetAccessPlayerCharacter() { return AccessPlayerCharacter; }
	
	UFUNCTION()
	ENPCType GetNPCType() const { return NPCType;}

	UFUNCTION()
	UQuestNPCComponent* GetQuestNPCComponent() const { return QuestNPCComponent;}

	UFUNCTION()
	virtual void OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	virtual void OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ENPCType NPCType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UWidgetComponent* InteractWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UQuestNPCComponent* QuestNPCComponent;
	
};
