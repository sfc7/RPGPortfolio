// LJS

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RPGGameInstance.generated.h"

/**
 * URPGGameInstance
 * 
 * 가장 기본 URPGGameInstance
 */
UCLASS()
class RPGPORTFOLIO_API URPGGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	//@ 첫 로드 여부 플래그 (세이브 시스템용)
	bool bFirstTimeLoadIn = true;

	// 플레이어 초기화 대기 타이머 설정
	float DelayTime = 0.3f; 
protected:
	//@ 맵 로딩 전 처리, 로딩 스크린 설정 등 
	virtual void OnPreLoadMap(const FString& MapName);
	
	//@ 목적지 월드 로딩 완료 후 처리, 로딩 스크린 종료 등
	virtual void OnDestinationWorldLoaded(UWorld* LoadedWorld);
};
