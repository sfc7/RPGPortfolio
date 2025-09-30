// LJS

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_RPGItemData.generated.h"

//@ 아이템 타입 - 아이템의 종류 구분
UENUM(BlueprintType)
enum class EItemType : uint8
{
	Material,
	Potion,
	Equipment,
	None
};

/**
 * UDataAsset_RPGItemData
 * 
 * 모든 아이템의 기본 데이터
 */
UCLASS()
class RPGPORTFOLIO_API UDataAsset_RPGItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UDataAsset_RPGItemData(const FObjectInitializer& ObjectInitializer);

	//@ 스택 가능 여부 확인
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsStackable();

	UFUNCTION(BlueprintCallable)
	FName GetItemID() const {return ItemID; }

	//@ 자체 반환
	UFUNCTION(BlueprintCallable)
	UDataAsset_RPGItemData* GetItemAsset() { return this; }

	//@ 아이템 고유 식별자
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName ItemID;
	
	//@ 아이템 아이콘 텍스처
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> ItemIcon;

	//@ 아이템 타입
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	EItemType ItemType = EItemType::None;

	//@ 아이템 표시용 이름
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FText ItemName;

	//@ 아이템 설명 텍스트
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FText ItemDescription;

	//@ 최대 스택 크기 (1이면 스택 불가능)
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 StackSize;

	//@ 아이템 골드 가치 (구매/판매용)
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 GoldValue;
};
