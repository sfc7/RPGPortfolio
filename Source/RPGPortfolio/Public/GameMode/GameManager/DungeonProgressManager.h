// LJS

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/DataTable.h"
#include "DungeonProgressManager.generated.h"

class AMonsterCharacter;

/**
 * 
 */
UENUM(BlueprintType)
enum class EDungeonState : uint8
{
	NormalMonsterPhase,
	BossMonsterPhase,
	Clear,
	GameOver,
};

UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	NormalMonster,
	BossMonster,
	None,
};

USTRUCT(BlueprintType)
struct FMonsterInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AMonsterCharacter> MonsterSoftClassPtr;

	UPROPERTY(EditAnywhere)
	FName MonsterName;
};

USTRUCT(BlueprintType)
struct FMonsterInDungeonTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FMonsterInfo MonsterInfo;

	UPROPERTY(EditAnywhere)
	EMonsterType MonsterType;
};

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDungeonStateChangedDelegate,EDungeonState,GamemodeState);

UCLASS()
class RPGPORTFOLIO_API UDungeonProgressManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UDungeonProgressManager();

	void SetDungeonState(EDungeonState GamemodeState);

	void AsyncLoadSoftClassptrMonster();

	void SpawnMonster();
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* MonsterDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSoftClassPtr<UUserWidget> GameOverUIWidgetClass;
	
	UPROPERTY()
	TMap<TSoftClassPtr<AMonsterCharacter>, UClass*> AsyncLoadMonsterMap;

	UPROPERTY(VisibleAnywhere)
	EDungeonState DungeonState;

	UPROPERTY(BlueprintAssignable,BlueprintCallable)
	FOnDungeonStateChangedDelegate OnDungeonStateChangedDelegate;

	UFUNCTION()
	void OnDungeonStateChanged(EDungeonState NewState);
	
	UFUNCTION()
	void MonsterKillCount(AActor* DestroyedActor);

	UPROPERTY()
	int32 NormalMonsterCount;

	bool SpawnFinish;
};


