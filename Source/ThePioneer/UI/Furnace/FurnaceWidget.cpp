// Fill out your copyright notice in the Description page of Project Settings.

#include "FurnaceWidget.h"
#include "Player/MyPlayerController.h"
#include "Object/Furnace/Furnace.h"

UTexture * UFurnaceWidget::ButtonNumber(int _num)
{
	if ((*(*Inven + _num)) != nullptr)
	{
		GItemData Data = (*(*Inven + _num))->Data;
		return MyGameInstance->ObjTexture[Data.ItemCode][Data.ItemAidCode][0];
	}

	return nullptr;
}

int UFurnaceWidget::ItemAmount(int _Number)
{
	if ((*(*Inven + _Number)) != nullptr)
	{
		GItemData Data = (*(*Inven + _Number))->Data;
		return Data.Amount;
	}

	return 0;
}

bool UFurnaceWidget::GetWear(int _Number)
{
	if ((*(*Inven + _Number)) != nullptr)
	{
		GObjDataBase Data = MyGameInstance->ObjDataBase[(*(*Inven + _Number))->Data.ItemCode][(*(*Inven + _Number))->Data.ItemAidCode];
		return Data.Can_Wear;
	}

	return true;
}

UTexture * UFurnaceWidget::CheckFurnace(int _Number)
{
	GItemDataNode * Data = nullptr;
	switch (_Number)
	{
	case 30:
		Data = ((Furnace*)FurnaceAdr)->GetMaterialItem(0);
		break;

	case 31:
		Data = ((Furnace*)FurnaceAdr)->GetMaterialItem(1);
		break;

	case 32:
		Data = ((Furnace*)FurnaceAdr)->GetMaterialItem(2);
		break;

	case 33:
		Data = ((Furnace*)FurnaceAdr)->GetResultItem();
		break;
	}

	if (Data)
		return MyGameInstance->ObjTexture[Data->Data.ItemCode][Data->Data.ItemAidCode][0];

	return nullptr;
}

int UFurnaceWidget::CheckFurnaceAmount(int _Number)
{
	GItemDataNode * Data = nullptr;
	switch (_Number)
	{
	case 30:
		Data = ((Furnace*)FurnaceAdr)->GetMaterialItem(0);
		break;

	case 31:
		Data = ((Furnace*)FurnaceAdr)->GetMaterialItem(1);
		break;

	case 32:
		Data = ((Furnace*)FurnaceAdr)->GetMaterialItem(2);
		break;

	case 33:
		Data = ((Furnace*)FurnaceAdr)->GetResultItem();
		break;
	}

	if (Data)
		return Data->Data.Amount;

	return 0;
}

void UFurnaceWidget::StartCrafting()
{
	((Furnace*)FurnaceAdr)->StartCrafting();
}

void UFurnaceWidget::TickA()
{
	if (FurnaceAdr != nullptr)
	{
		FurnaceOpen.Broadcast();
	}
}

void UFurnaceWidget::FurnaceWidget()
{
	MyGameInstance = Cast<UMyGameInstance>(GetGameInstance());
}

void UFurnaceWidget::OpenFurnace()
{
	FurnaceOpen.Broadcast();
}

void UFurnaceWidget::SetInventory(GItemDataNode *(*_Inven)[INVEN_ROW])
{
	for (int i = 0; i < INVEN_SIZE; i++)
		*(*Inven + i) = *(*_Inven + i);
}

void UFurnaceWidget::SetFurnace(ObjectDataClass * _FurnaceAdr)
{
	FurnaceAdr = _FurnaceAdr;
}

void UFurnaceWidget::SwapInven(int _Number, int _NewNumber)
{
	if (_Number != _NewNumber)
	{
		GObjDataBase Data;
		if (_Number < 30 && (*(*Inven + _NewNumber)))
			Data = MyGameInstance->ObjDataBase[(*(*Inven + _Number))->Data.ItemCode][(*(*Inven + _Number))->Data.ItemAidCode];

		else if (_NewNumber < 30 && (*(*Inven + _NewNumber)))
			Data = MyGameInstance->ObjDataBase[(*(*Inven + _NewNumber))->Data.ItemCode][(*(*Inven + _NewNumber))->Data.ItemAidCode];

		else
			Data.Can_Wear = false;

		if (!Data.Can_Wear)
		{
			if ((_NewNumber >= 30 && _NewNumber <= 32) && (_Number < 30)) //인벤에서 재료칸으로 이동
			{
				((Furnace*)FurnaceAdr)->SetMaterialItem(*(*Inven + _Number), _NewNumber - 30);

				*(*Inven + _Number) = nullptr;
			}

			else if ((_NewNumber >= 30 && _NewNumber <= 32) && (_Number >= 30 && _Number <= 32)) //재료칸 끼리 이동
			{
				GItemDataNode * Num;
				GItemDataNode * NewNum;

				Num = ((Furnace*)FurnaceAdr)->GetMaterialItem(_Number - 30);

				NewNum = ((Furnace*)FurnaceAdr)->GetMaterialItem(_NewNumber - 30);

				if (Num != nullptr && NewNum != nullptr)
				{
					if (Num->Data.ItemCode == NewNum->Data.ItemCode && Num->Data.ItemAidCode == NewNum->Data.ItemAidCode)
					{
						Num->Data.Amount += NewNum->Data.Amount;
						delete(NewNum);
					}
					else
					{
						((Furnace*)FurnaceAdr)->SetMaterialItem(Num, _NewNumber - 30);
						((Furnace*)FurnaceAdr)->SetMaterialItem(NewNum, _Number - 30);
					}
				}
				else
				{
					((Furnace*)FurnaceAdr)->SetMaterialItem(Num, _NewNumber - 30);
					((Furnace*)FurnaceAdr)->SetMaterialItem(NewNum, _Number - 30);
				}
			}

			else if (_Number >= 30 && _NewNumber < 30) //인벤으로 이동
			{
				GItemData InvenData;
				GItemData FurnaceData;

				switch (_Number)
				{
				case 30:
					if ((*(*Inven + _NewNumber)) != nullptr)
					{
						InvenData = (*(*Inven + _NewNumber))->Data;
						FurnaceData = ((Furnace*)FurnaceAdr)->GetMaterialItem(0)->Data;

						if (InvenData.ItemCode == FurnaceData.ItemCode &&
							InvenData.ItemAidCode == FurnaceData.ItemAidCode)
						{
							(*(*Inven + _NewNumber))->Data.Amount += ((Furnace*)FurnaceAdr)->GetMaterialItem(0)->Data.Amount;
							delete(((Furnace*)FurnaceAdr)->GetMaterialItem(0));
							((Furnace*)FurnaceAdr)->SetMaterialItem(nullptr, 0);
						}
						else
						{
							*(*Inven + _NewNumber) = ((Furnace*)FurnaceAdr)->GetMaterialItem(0);
							((Furnace*)FurnaceAdr)->SetMaterialItem(nullptr, 0);
						}
					}
					else
					{
						*(*Inven + _NewNumber) = ((Furnace*)FurnaceAdr)->GetMaterialItem(0);
						((Furnace*)FurnaceAdr)->SetMaterialItem(nullptr, 0);
					}
					break;

				case 31:
					if ((*(*Inven + _NewNumber)) != nullptr)
					{
						InvenData = (*(*Inven + _NewNumber))->Data;
						FurnaceData = ((Furnace*)FurnaceAdr)->GetMaterialItem(1)->Data;

						if (InvenData.ItemCode == FurnaceData.ItemCode &&
							InvenData.ItemAidCode == FurnaceData.ItemAidCode)
						{
							(*(*Inven + _NewNumber))->Data.Amount += ((Furnace*)FurnaceAdr)->GetMaterialItem(1)->Data.Amount;
							delete(((Furnace*)FurnaceAdr)->GetMaterialItem(1));
							((Furnace*)FurnaceAdr)->SetMaterialItem(nullptr, 1);
						}
						else
						{
							*(*Inven + _NewNumber) = ((Furnace*)FurnaceAdr)->GetMaterialItem(1);
							((Furnace*)FurnaceAdr)->SetMaterialItem(nullptr, 1);
						}
					}
					else
					{
						*(*Inven + _NewNumber) = ((Furnace*)FurnaceAdr)->GetMaterialItem(1);
						((Furnace*)FurnaceAdr)->SetMaterialItem(nullptr, 1);
					}
					break;

				case 32:
					if ((*(*Inven + _NewNumber)) != nullptr)
					{
						InvenData = (*(*Inven + _NewNumber))->Data;
						FurnaceData = ((Furnace*)FurnaceAdr)->GetMaterialItem(2)->Data;

						if (InvenData.ItemCode == FurnaceData.ItemCode &&
							InvenData.ItemAidCode == FurnaceData.ItemAidCode)
						{
							(*(*Inven + _NewNumber))->Data.Amount += ((Furnace*)FurnaceAdr)->GetMaterialItem(2)->Data.Amount;
							delete(((Furnace*)FurnaceAdr)->GetMaterialItem(2));
							((Furnace*)FurnaceAdr)->SetMaterialItem(nullptr, 2);
						}
						else
						{
							*(*Inven + _NewNumber) = ((Furnace*)FurnaceAdr)->GetMaterialItem(2);
							((Furnace*)FurnaceAdr)->SetMaterialItem(nullptr, 2);
						}
					}
					else
					{
						*(*Inven + _NewNumber) = ((Furnace*)FurnaceAdr)->GetMaterialItem(2);
						((Furnace*)FurnaceAdr)->SetMaterialItem(nullptr, 2);
					}
					break;

				case 33:
					if ((*(*Inven + _NewNumber)) != nullptr)
					{
						InvenData = (*(*Inven + _NewNumber))->Data;
						FurnaceData = ((Furnace*)FurnaceAdr)->GetResultItem()->Data;

						if (InvenData.ItemCode == FurnaceData.ItemCode &&
							InvenData.ItemAidCode == FurnaceData.ItemAidCode)
						{
							(*(*Inven + _NewNumber))->Data.Amount += ((Furnace*)FurnaceAdr)->GetResultItem()->Data.Amount;
							delete(((Furnace*)FurnaceAdr)->GetResultItem());
							((Furnace*)FurnaceAdr)->SetResultItem(nullptr);
						}
						else
						{
							*(*Inven + _NewNumber) = ((Furnace*)FurnaceAdr)->GetResultItem();
							((Furnace*)FurnaceAdr)->SetResultItem(nullptr);
						}
					}
					else
					{
						*(*Inven + _NewNumber) = ((Furnace*)FurnaceAdr)->GetResultItem();
						((Furnace*)FurnaceAdr)->SetResultItem(nullptr);
					}
					break;
				}
			}

			else if (_Number < 30 && _NewNumber < 30) //인벤끼리 이동
			{
				GItemDataNode * Save;
				GItemDataNode ** Num;
				GItemDataNode ** NewNum;

				Num = &(*(*Inven + _Number));
				NewNum = &(*(*Inven + _NewNumber));
				Save = *NewNum;

				if ((*Num)->Data.ItemCode == (*NewNum)->Data.ItemCode &&
					(*Num)->Data.ItemAidCode == (*NewNum)->Data.ItemAidCode)
				{
					(*(*Inven + _NewNumber))->Data.Amount += (*(*Inven + _Number))->Data.Amount;
					delete((*(*Inven + _Number)));
				}
				else
				{
					*NewNum = *Num;
					*Num = Save;
				}
			}

			Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController())->SetInven(Inven);
			FurnaceOpen.Broadcast();
		}
	}
}
