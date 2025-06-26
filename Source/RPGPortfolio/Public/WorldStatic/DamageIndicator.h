// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageIndicator.generated.h"

class UWidgetComponent;

UCLASS()
class RPGPORTFOLIO_API ADamageIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	ADamageIndicator();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent* DamageWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ShowDamage;

	UFUNCTION(BlueprintCallable)
	void SetWidgetLocation();

	UFUNCTION(BlueprintCallable)
	void SetDamage(float Damage);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector StartPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector EndPosition;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
