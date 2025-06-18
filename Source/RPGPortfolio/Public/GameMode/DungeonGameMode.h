// LJS

#pragma once

#include "CoreMinimal.h"
#include "Character/MonsterCharacter.h"
#include "GameMode/RPGGameModeBase.h"
#include "DungeonGameMode.generated.h"

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
	PlayerDeath,
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



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDungeonStateChangedDelegate,EDungeonState,GamemodeState);

UCLASS()
class RPGPORTFOLIO_API ADungeonGameMode : public ARPGGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void SetDungeonState(EDungeonState GamemodeState);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* MonsterDataTable;
	
	void AsyncLoadSoftClassptrMonster();

	void SpawnMonster();
		
	UPROPERTY()
	TMap<TSoftClassPtr<AMonsterCharacter>, UClass*> AsyncLoadMonsterMap;
	
	UPROPERTY(VisibleAnywhere)
	EDungeonState DungeonState;

	UPROPERTY(BlueprintAssignable,BlueprintCallable)
	FOnDungeonStateChangedDelegate OnDungeonStateChangedDelegate;

	UPROPERTY()
	float GameTime;

	bool flagspawn;
};
