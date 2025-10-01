// LJS

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGFunc.generated.h"

struct FScalableFloat;
class UGeneralGameManager;

/**
 * URPGFunc
 * 
 * 유틸리티 함수 라이브러리
 */
UCLASS()
class RPGPORTFOLIO_API URPGFunc : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//@ 소스와 타겟이 서로 다른 팀인지 확인
	UFUNCTION(BlueprintPure)
	static bool CheckTargetTeamAgent(APawn* SourcePawn, APawn* TargetPawn);

	//@ FScalableFloat에서 레벨에 따른 값 가져오기
	UFUNCTION(BlueprintPure)
	static float GetScalableFloatconst (const FScalableFloat& ScalableFloat, float Level = 1.f);

	//@ 공격자와 피격자 간의 히트 방향 계산
	UFUNCTION(BlueprintPure)
	static FGameplayTag GetHitReactDirection (AActor* Attacker, AActor* HitReactActor, float& OutHitReactDirection);

	//@ 방어자의 방어가 유효한지 확인 (벡터 내적으로)
	UFUNCTION(BlueprintPure)
	static bool IsValidDefense(AActor* Attacker, AActor* Defender);

	// 스킬 쿨타임 아이콘으로 나타내기 위해 만듬, 커스텀한 카운트다운 함수 사용
	UFUNCTION(BlueprintCallable, meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo", ExpandEnumAsExecs = "CountDownInput|CountDownOutput",TotalTime = "1.0",UpdateInterval = "0.1"))
	static void CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval, float& OutRemainingTime, ERPGCountDownActionInput CountDownInput,  UPARAM(DisplayName = "Output") ERPGCountDownActionOutput& CountDownOutput, FLatentActionInfo LatentInfo);

	//@ 통괄 게임 매니저 가져오기
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static UGeneralGameManager* GetGeneralGameManager(const UObject* WorldContextObject);
	
};
