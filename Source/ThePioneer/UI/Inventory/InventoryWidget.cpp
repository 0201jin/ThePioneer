// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryWidget.h"
#include "Player/MyPlayerController.h"

void UInventoryWidget::InventoryWidget()
{
	MyGameInstance = Cast<UMyGameInstance>(GetGameInstance());

	for (auto It = MyGameInstance->CraftDataBase.CreateConstIterator(); It; It++)
	{
		CraftDataBase.Add(*It);
	}
}

void UInventoryWidget::OpenInven()
{
	InvenOpen.Broadcast();
}

void UInventoryWidget::WorkBenchWidget()
{
	MyGameInstance = Cast<UMyGameInstance>(GetGameInstance());
}

void UInventoryWidget::OpenWorkBench(TArray<FCraftItemList> _CraftList)
{
	CraftList = _CraftList;
	WorkBenchOpen.Broadcast(_CraftList);
}

void UInventoryWidget::CloseWorkBench()
{
	WorkBenchClose.Broadcast();
}

void UInventoryWidget::SetInventory(GItemDataNode *(*_Inven)[INVEN_ROW])
{
	for (int i = 0; i < INVEN_SIZE; i++)
		*(*Inven + i) = *(*_Inven + i);
}

void UInventoryWidget::SetItemMaterial(int(*_ItemMaterial)[256])
{
	for (int i = 0; i < (4 * 256); i++)
		*(*ItemMaterial + i) = *(*_ItemMaterial + i);
}

UTexture * UInventoryWidget::GetTexture(int _ItemCode, int _ItemAidCode)
{
	return MyGameInstance->ObjTexture[_ItemCode][_ItemAidCode][0];
}

int UInventoryWidget::GetMatAmount(int _ItemCode, int _ItemAidCode)
{
	return ItemMaterial[_ItemCode][_ItemAidCode];
}

void UInventoryWidget::CraftItem(TArray<FItemMaterial> _MaterList, FItemMaterial _CraftItem)
{
	if (_CraftItem.Amount > 0)
	{
		for (int i = 0; i < _CraftItem.Amount; i++)
		{
			bool bCanCraft = false;

			for (auto It = _MaterList.CreateConstIterator(); It; It++) //아이템 제작 리스트
			{
				if ((*It).Amount > ItemMaterial[(*It).ItemCode][(*It).ItemAidCode])
				{
					bCanCraft = false;
					return;
				}

				bCanCraft = true;
			}

			//인벤 공간 체크
			bCanCraft = false;
			int Max_Amount = MyGameInstance->ObjDataBase[_CraftItem.ItemCode][_CraftItem.ItemAidCode].Max_Amount;

			if (!MyGameInstance->ObjDataBase[_CraftItem.ItemCode][_CraftItem.ItemAidCode].Can_Wear)
			{
				for (int i = 0; i < INVEN_SIZE; i++) //겹치는 코드
				{
					if ((*(*Inven + i)) != nullptr)
					{
						if (((*(*Inven + i))->Data.ItemCode != 0 && (*(*Inven + i))->Data.ItemAidCode != 0))
						{
							GItemData InvenData = (*(*Inven + i))->Data;

							int Amount = (*(*Inven + i))->Data.Amount + 1;

							if (InvenData.ItemCode == _CraftItem.ItemCode &&
								InvenData.ItemAidCode == _CraftItem.ItemAidCode &&
								Amount <= Max_Amount)
							{
								bCanCraft = true;
								break;
							}

							if (InvenData.ItemCode == _CraftItem.ItemCode &&
								InvenData.ItemAidCode == _CraftItem.ItemAidCode &&
								(*(*Inven + i))->Data.Amount < Max_Amount)
							{
								bCanCraft = true;
								break;
							}
						}
					}
				}
			}

			for (int a = 0; a < INVEN_SIZE; a++) //안겹치는 코드
			{
				if ((*(*Inven + a)) == nullptr)
				{
					bCanCraft = true;
					break;
				}

				else if (((*(*Inven + a))->Data.ItemCode == 0 && (*(*Inven + a))->Data.ItemAidCode == 0))
				{
					bCanCraft = true;
					break;
				}
			}

			if (!bCanCraft)
				return;

			//재료 삭제
			for (auto It = _MaterList.CreateConstIterator(); It; It++) //아이템 제작 리스트
			{
				int NeedMater = (*It).Amount;
				for (int i = 0; i < INVEN_SIZE; i++)
				{
					if ((*(*Inven + i)))
					{
						GItemData Data = (*(*Inven + i))->Data;
						if (Data.ItemCode == (*It).ItemCode &&
							Data.ItemAidCode == (*It).ItemAidCode)
						{
							int Amount = (*(*Inven + i))->Data.Amount - ((*It).Amount);
							(*(*Inven + i))->Data.Amount -= (*It).Amount;
							ItemMaterial[Data.ItemCode][Data.ItemAidCode] -= (*It).Amount;
							NeedMater -= (*It).Amount;

							if (ItemMaterial[Data.ItemCode][Data.ItemAidCode] <= 0)
								ItemMaterial[Data.ItemCode][Data.ItemAidCode] = 0;

							if (Amount <= 0)
							{
								memset((*(*Inven + i)), 0, sizeof((*(*Inven + i))));
							}

							if (NeedMater <= 0)
								break;
						}
					}
				}
			}

			//CraftItem을 인벤에 추가
			GItemDataNode * CraftItem = new GItemDataNode;

			CraftItem->Data.NodeAdr = CraftItem;
			CraftItem->Data.Amount = 1;
			CraftItem->Data.ItemCode = _CraftItem.ItemCode;
			CraftItem->Data.ItemAidCode = _CraftItem.ItemAidCode;
			CraftItem->Data.Attack = MyGameInstance->ObjDataBase[_CraftItem.ItemCode][_CraftItem.ItemAidCode].Attack;
			CraftItem->Data.Distance = MyGameInstance->ObjDataBase[_CraftItem.ItemCode][_CraftItem.ItemAidCode].Distance;
			CraftItem->Data.Sheild = MyGameInstance->ObjDataBase[_CraftItem.ItemCode][_CraftItem.ItemAidCode].Sheild;
			CraftItem->Data.Health = MyGameInstance->ObjDataBase[_CraftItem.ItemCode][_CraftItem.ItemAidCode].Health;

			Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController())->CraftItem(CraftItem);

			//재료 상황 실시간 반영
			CraftList.Empty();

			for (auto It = CraftDataBase.CreateConstIterator(); It; It++) //아이템 제작 리스트
			{
				FCraftItemList CraftData;

				CraftData.ItemCode = (*It).ItemCode;
				CraftData.ItemAidCode = (*It).ItemAidCode;
				CraftData.ItemName = MyGameInstance->ObjName[(*It).ItemCode][(*It).ItemAidCode].ToString();
				CraftData.Texture = MyGameInstance->ObjTexture[(*It).ItemCode][(*It).ItemAidCode][0];
				CraftData.ResourceNeed = (*It).ResourceNeed;

				for (auto Iter = (*It).ResourceNeed.CreateConstIterator(); Iter; Iter++) //필요한 재료 리스트
				{
					if ((*Iter).Amount > ItemMaterial[(*Iter).ItemCode][(*Iter).ItemAidCode])
					{
						CraftData.CanMake = false;
						break;
					}

					CraftData.CanMake = true;
				}

				CraftList.Add(CraftData);
			}
		}
		OpenWorkBench(CraftList);
	}
}

UTexture * UInventoryWidget::GetHeadTexture()
{
	AMyPlayerController * PC = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());

	if (PC->GetHead() == nullptr)
	{
		return nullptr;
	}

	else if (PC->GetHead()->Data.ItemCode == 0 && PC->GetHead()->Data.ItemAidCode == 0)
	{
		return nullptr;
	}

	return MyGameInstance->ObjTexture[PC->GetHead()->Data.ItemCode][PC->GetHead()->Data.ItemAidCode][0];
}

UTexture * UInventoryWidget::GetBodyTexture()
{
	AMyPlayerController * PC = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());

	if (PC->GetBody() == nullptr)
	{
		return nullptr;
	}

	else if (PC->GetBody()->Data.ItemCode == 0 && PC->GetBody()->Data.ItemAidCode == 0)
	{
		return nullptr;
	}

	return MyGameInstance->ObjTexture[PC->GetBody()->Data.ItemCode][PC->GetBody()->Data.ItemAidCode][0];
}

UTexture * UInventoryWidget::GetShooseTexture()
{
	AMyPlayerController * PC = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());

	if (PC->GetShoose() == nullptr)
	{
		return nullptr;
	}

	else if (PC->GetShoose()->Data.ItemCode == 0 && PC->GetShoose()->Data.ItemAidCode == 0)
	{
		return nullptr;
	}

	return MyGameInstance->ObjTexture[PC->GetShoose()->Data.ItemCode][PC->GetShoose()->Data.ItemAidCode][0];
}

bool UInventoryWidget::GetWear(int _Number)
{
	if ((*(*Inven + _Number)) != nullptr)
	{
		GObjDataBase Data = MyGameInstance->ObjDataBase[(*(*Inven + _Number))->Data.ItemCode][(*(*Inven + _Number))->Data.ItemAidCode];
		return Data.Can_Wear;
	}

	return true;
}

UTexture * UInventoryWidget::ButtonNumber(int _num)
{
	if ((*(*Inven + _num)) != nullptr)
	{
		GItemData Data = (*(*Inven + _num))->Data;
		return MyGameInstance->ObjTexture[Data.ItemCode][Data.ItemAidCode][0];
	}

	return nullptr;
}

int UInventoryWidget::ItemAmount(int _Number)
{
	if ((*(*Inven + _Number)) != nullptr)
	{
		GItemData Data = (*(*Inven + _Number))->Data;
		return Data.Amount;
	}

	return 0;
}