// LJS

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGFunc.generated.h"

struct FScalableFloat;
class UGeneralGameManager;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGFunc : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static bool CheckTargetTeamAgent(APawn* SourcePawn, APawn* TargetPawn);

	UFUNCTION(BlueprintPure)
	static float GetScalableFloatconst (const FScalableFloat& ScalableFloat, float Level = 1.f);

	UFUNCTION(BlueprintPure)
	static FGameplayTag GetHitReactDirection (AActor* Attacker, AActor* HitReactActor, float& OutHitReactDirection);

	UFUNCTION(BlueprintPure)
	static bool IsValidDefense(AActor* Attacker, AActor* Defender);

	UFUNCTION(BlueprintCallable)
	static bool ApplyGameplayEffectSpecHandleToTargetActor(AActor* Instigator,AActor* TargetActor,const FGameplayEffectSpecHandle& SpecHandle);

	UFUNCTION(BlueprintCallable, meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo", ExpandEnumAsExecs = "CountDownInput|CountDownOutput",TotalTime = "1.0",UpdateInterval = "0.1"))
	static void CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval, float& OutRemainingTime, ERPGCountDownActionInput CountDownInput,  UPARAM(DisplayName = "Output") ERPGCountDownActionOutput& CountDownOutput, FLatentActionInfo LatentInfo);

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static UGeneralGameManager* GetGeneralGameManager(const UObject* WorldContextObject);
	
};
