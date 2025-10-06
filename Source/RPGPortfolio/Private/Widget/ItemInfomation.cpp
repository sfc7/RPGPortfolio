// LJS


#include "Widget/ItemInfomation.h"

#include "GameMode/GameManager/ItemManager.h"
#include "DataAsset/Item/DataAsset_RPGItemData.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DataAsset/Item/DataAsset_RPGItemData_Equipment.h"
#include "DataAsset/Item/DataAsset_RPGItemData_Potion.h"

void UItemInfomation::UpdateSlotData(FInventorySlot UpdateSlotData)
{
	UDataAsset_RPGItemData* ItemDataAssetObject = UpdateSlotData.ItemDataAsset.LoadSynchronous();
	ItemImage->SetBrushFromSoftTexture(ItemDataAssetObject->ItemIcon);
	ItemName->SetText(FText::FromName(UpdateSlotData.ItemID));


	FText ItemDescriptionText = GetItemDescription(UpdateSlotData.ItemDataAsset);
	ItemDescription->SetText(ItemDescriptionText);
}

FText UItemInfomation::GetItemDescription(TSoftObjectPtr<UDataAsset_RPGItemData> ItemData)
{
	UDataAsset_RPGItemData* ItemDataLoad = ItemData.LoadSynchronous();
	FText ResultText;
	
	if (ItemDataLoad)
	{
		// 아이템 타입별 설명 생성
		switch (ItemDataLoad->ItemType)
		{
		case EItemType::Material:
			{
				ResultText = FText::Format(
					NSLOCTEXT("ItemInfo", "MaterialDescription", 
						"{0}\n가격: {1} 골드"),
					ItemDataLoad->ItemDescription,
					FText::AsNumber(ItemDataLoad->GoldValue)
				);
			}
			break;
			
		case EItemType::Potion:
			{
				UDataAsset_RPGItemData_Potion* PotionData = Cast<UDataAsset_RPGItemData_Potion>(ItemDataLoad);
				if (PotionData)
				{
					ResultText = FText::Format(
						NSLOCTEXT("ItemInfo", "PotionDescription", 
							"{0}\n회복량: {1} HP\n가격: {2} 골드"),
						ItemDataLoad->ItemDescription,
						FText::AsNumber(PotionData->HealAmount),
						FText::AsNumber(ItemDataLoad->GoldValue)
					);
				}
			}
			break;
			// 장비 타입 텍스트 생성
		case EItemType::Equipment:
			{
				UDataAsset_RPGItemData_Equipment* EquipmentData = Cast<UDataAsset_RPGItemData_Equipment>(ItemDataLoad);
				if (EquipmentData)
				{
					FText EquipmentTypeText;
					switch (EquipmentData->EquipmentType)
					{
					case EEquipmentType::Helmet:
						EquipmentTypeText = NSLOCTEXT("EquipmentType", "Helmet", "헬멧");
						break;
					case EEquipmentType::Armor:
						EquipmentTypeText = NSLOCTEXT("EquipmentType", "Armor", "갑옷");
						break;
					case EEquipmentType::Pants:
						EquipmentTypeText = NSLOCTEXT("EquipmentType", "Pants", "바지");
						break;
					case EEquipmentType::Gloves:
						EquipmentTypeText = NSLOCTEXT("EquipmentType", "Gloves", "장갑");
						break;
					case EEquipmentType::Boots:
						EquipmentTypeText = NSLOCTEXT("EquipmentType", "Boots", "부츠");
						break;
					default:
						EquipmentTypeText = NSLOCTEXT("EquipmentType", "Unknown", "알 수 없음");
						break;
					}
					
					FString DescriptionText = FString::Printf(TEXT("%s\n장비 타입: %s"), 
						*ItemDataLoad->ItemDescription.ToString(), 
						*EquipmentTypeText.ToString());
					
					// 능력치 정보 추가
					if (EquipmentData->MaxHp > 0)
					{
						DescriptionText += FString::Printf(TEXT("\n최대 HP: +%.0f"), EquipmentData->MaxHp);
					}
					
					if (EquipmentData->MaxMp > 0)
					{
						DescriptionText += FString::Printf(TEXT("\n최대 MP: +%.0f"), EquipmentData->MaxMp);
					}
					
					if (EquipmentData->AttackRate > 0)
					{
						DescriptionText += FString::Printf(TEXT("\n공격력: +%.1f"), EquipmentData->AttackRate);
					}
					
					if (EquipmentData->Defense > 0)
					{
						DescriptionText += FString::Printf(TEXT("\n방어력: +%.0f"), EquipmentData->Defense);
					}
					
					DescriptionText += FString::Printf(TEXT("\n가격: %d 골드"), ItemDataLoad->GoldValue);
					
					ResultText = FText::FromString(DescriptionText);
				}
			}
			break;
			
		default:
			{
				ResultText = ItemDataLoad->ItemDescription;
			}
			break;
		}
	}

	return ResultText;
}

void UItemInfomation::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UItemInfomation::NativePreConstruct()
{
	Super::NativePreConstruct();
}
