// LJS

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/DataTable.h"
#include "DungeonProgressManager.generated.h"

class AMonsterCharacter;
class ASpawningVolume;
class ADungeonGameMode;

//@ 던전 상태 열거형
UENUM(BlueprintType)
enum class EDungeonState : uint8
{
	NormalMonsterPhase,
	BossMonsterPhase,
	Clear,
	GameOver,
};

//@ 몬스터 스폰에 쓸 열거형
UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	NormalMonster,
	BossMonster,
	None,
};

//@ 몬스터 정보 구조체
USTRUCT(BlueprintType)
struct FMonsterInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AMonsterCharacter> MonsterSoftClassPtr;

	UPROPERTY(EditAnywhere)
	FName MonsterName;
};

//@ 몬스터 스폰 데이터 테이블 
USTRUCT(BlueprintType)
struct FMonsterSpawnTable : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere)
	FMonsterInfo MonsterInfo;

	UPROPERTY(EditAnywhere)
	EMonsterType MonsterType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MonsterLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator SpawnRotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SpawningVolumeName;
};

//@ 던전 상태 변경 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDungeonStateChangedDelegate,EDungeonState,GamemodeState);

/**
 * UDungeonProgressManager
 * 
 * 던전 진행 관리 서브시스템
 */
UCLASS()
class RPGPORTFOLIO_API UDungeonProgressManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UDungeonProgressManager();

	//@ 던전 상태 설정 및 브로드캐스트
	void SetDungeonState(EDungeonState GamemodeState);

	//@ 소프트 클래스 포인터 몬스터 비동기 로딩
	void AsyncLoadSoftClassptrMonster();

	//@ 일반 몬스터 스폰
	void SpawnMonster(ASpawningVolume& SpawningVolume);

	//@ 보스 몬스터 스폰
	void SpawnBossMonster(ASpawningVolume& SpawningVolume);
	
	//@ 몬스터 스폰 볼륨 등록
	void RegisterSpawningVolume(ASpawningVolume *SpawningVolume);

	//@ 이름으로 스폰 볼륨 찾기
	ASpawningVolume* FindSpawningVolumebyName(FString SpawningVolumeName) const;
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	//@ 몬스터 스폰 데이터 테이블 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* MonsterSpawnTable;

	//@ 게임 오버 UI 위젯 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSoftClassPtr<UUserWidget> GameOverUIWidgetClass;

	//@ 비동기 로드된 몬스터 클래스 맵
	UPROPERTY()
	TMap<TSoftClassPtr<AMonsterCharacter>, UClass*> AsyncLoadMonsterMap;

	//@ 스폰 볼륨 맵
	UPROPERTY()
	TMap<FString, ASpawningVolume*> SpawningVolumes;

	//@ 현재 던전 상태
	UPROPERTY(VisibleAnywhere)
	EDungeonState DungeonState;

	//@ 던전 상태 변경 델리게이트
	UPROPERTY(BlueprintAssignable,BlueprintCallable)
	FOnDungeonStateChangedDelegate OnDungeonStateChangedDelegate;

	//@ 던전 상태 변경 시 호출되는 콜백 함수
	UFUNCTION()
	void OnDungeonStateChanged(EDungeonState NewState);

	//@ 일반 몬스터 킬 카운트 처리
	UFUNCTION()
	void MonsterKillCount(AActor* DestroyedActor);

	//@ 보스 몬스터 킬 처리
	UFUNCTION()
	void BossMonsterKilled(AActor* DestroyedActor);

	//@ 일반 몬스터 수
	UPROPERTY()
	int32 NormalMonsterCount;

	//@ 보스 몬스터 수
	UPROPERTY()
	int32 BossMonsterCount;

	//@ 스폰 완료 플래그
	bool SpawnFinish;

	//@ 보스 스폰 완료 플래그
	bool BossSpawnFinish;


};

