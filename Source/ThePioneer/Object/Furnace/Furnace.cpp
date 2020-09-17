// Fill out your copyright notice in the Description page of Project Settings.

#include "Furnace.h"
#include "Engine.h"
#include "Player/MyPlayerController.h"

Furnace::Furnace()
{
}

Furnace::~Furnace()
{
}

void Furnace::Tick(float DeltaTime, int x, int y)
{
	bCanCraft = false;

	GItemData Mater[3];
	for (auto Iter = Item_Material_Array.CreateConstIterator(); Iter; Iter++)
	{
		Mater[Iter.GetIndex()].ItemCode = (*Iter).ItemCode;
		Mater[Iter.GetIndex()].ItemAidCode = (*Iter).ItemAidCode;
		Mater[Iter.GetIndex()].Amount = (*Iter).Amount;
	}

	for (int m = 0; m < 3; m++)
	{
		for (int i = 0; i < 3; i++)
		{
			if (Material_Item[i] != nullptr)
			{
				if (Material_Item[i]->Data.Amount < Mater[m].Amount)
					Mater[m].Amount -= Material_Item[i]->Data.Amount - Mater[m].Amount;
				else
					Mater[m].Amount = 0;
			}
		}

		if (Mater[m].Amount != 0)
		{
			bCanCraft = false;
			break;
		}

		bCanCraft = true;
	}

	if (Crafting_Time > 0 && bCanCraft)
	{
		Crafting_Time -= DeltaTime;

		Parent->PointLight->Activate(true);
		Parent->PointLight->SetAttenuationRadius(100);

		if (Crafting_Time_All - Crafting_Time_One >= Crafting_Time)
		{
			//재료 수 빼기
			for (auto Iter = Item_Material_Array.CreateConstIterator(); Iter; Iter++)
			{
				Mater[Iter.GetIndex()].ItemCode = (*Iter).ItemCode;
				Mater[Iter.GetIndex()].ItemAidCode = (*Iter).ItemAidCode;
				Mater[Iter.GetIndex()].Amount = (*Iter).Amount;
			}

			for (int m = 0; m < 3; m++)
			{
				for (int i = 0; i < 3; i++)
				{
					if (Material_Item[m] != nullptr)
					{
						Material_Item[m]->Data.Amount -= Mater[i].Amount;

						if (Material_Item[m]->Data.Amount < 0)
						{
							Mater[i].Amount -= Material_Item[m]->Data.Amount;
							Material_Item[m]->Data.Amount = 0;
						}

						if (Material_Item[m]->Data.Amount == 0)
						{
							delete(Material_Item[m]);
							Material_Item[m] = nullptr;
						}

						Mater[i].Amount -= Mater[i].Amount;

						if (Mater[i].Amount <= 0)
							break;
					}

					else
						break;
				}
			}

			if (Result_Item == nullptr || (Result_Item->Data.ItemCode == 0 && Result_Item->Data.ItemAidCode))
			{
				GObjDataBase DataBase = MyGameInstance->ObjDataBase[CraftItemData.ItemCode][CraftItemData.ItemAidCode];

				CraftItemData.Amount = 1;
				CraftItemData.Attack = DataBase.Attack;
				CraftItemData.Distance = DataBase.Distance;
				CraftItemData.Health = DataBase.Health;
				CraftItemData.Sheild = DataBase.Sheild;

				Result_Item = CreateItemDataNode(CraftItemData);
			}

			else
				Result_Item->Data.Amount += 1;

			Crafting_Time_All -= Crafting_Time_One;
		}
	}

	else
	{
		bFindCraftItem = false;
		Parent->SetActorTickEnabled(false);
		Parent->PointLight->Activate(false);
		Parent->PointLight->SetAttenuationRadius(0);
	}
}

void Furnace::SetInGameLevel(AInGameLevel * _InGameLevel)
{
	InGameLevel = _InGameLevel;
	MyGameInstance = _InGameLevel->MyGameInstance;
}

void Furnace::SetParent(AActor * _parent)
{
	Parent = Cast<AObjectClass>(_parent);

	if (Parent)
	{
		//Parent->PointLight->SetAttenuationRadius(100);
		Parent->SetActorLocation(FVector(Parent->GetActorLocation().X, Parent->GetActorLocation().Y, 155));
		Parent->SetActorScale3D(FVector(0.25, 0.25, 0.25));
		Parent->SetActorRotation(FRotator(0, 90, 0));
	}
}

void Furnace::Action(APlayerController * _Controller)
{
	Cast<AMyHUD>(_Controller->GetHUD())->SetFurnace(this);
	Cast<AMyHUD>(_Controller->GetHUD())->SetInventory(Cast<AMyPlayerController>(_Controller)->GetInven());
	Cast<AMyHUD>(_Controller->GetHUD())->SwitchFurnace();
}

void Furnace::SetData(GObjectData * _Data)
{
	Data = _Data;
}

GObjectData * Furnace::GetData()
{
	return Data;
}

void Furnace::SetMaterialItem(GItemDataNode * _Material, int _Num)
{
	Material_Item[_Num] = _Material;
}

void Furnace::SetResultItem(GItemDataNode * _Result)
{
	Result_Item = _Result;
}

GItemDataNode * Furnace::GetMaterialItem(int _Num)
{
	if (Material_Item[_Num])
		return Material_Item[_Num];

	return nullptr;
}

GItemDataNode * Furnace::GetResultItem()
{
	if (Result_Item)
	{
		UE_LOG(LogTemp, Warning, TEXT("Result_Item"));
		return Result_Item;
	}

	return nullptr;
}

bool Furnace::GetActive()
{
	return true;
}

void Furnace::Damage(int _Damage)
{
	Health -= _Damage;

	if (Health <= 0)
	{
		int x = Parent->GetActorLocation().X * 0.01;
		int y = Parent->GetActorLocation().Y * 0.01;

		//아이템 스폰하는 코드
		for (int i = 0; i < 3; i++)
		{
			if (Material_Item[i] != nullptr)
			{
				InGameLevel->ItemSpawn(x, y, Material_Item[i]);
			}
		}

		if (Result_Item != nullptr)
		{
			InGameLevel->ItemSpawn(x, y, Result_Item);
		}

		InGameLevel->DeleteObjChunk(x, y);
	}
}

void Furnace::InitHealth()
{
	//여기에 보이지 않는 상태에서 동안의 계산을 진행
	if (Crafting_Time != 0)
	{
		int PassTime = InGameLevel->GetPlayTime() - Data->Time;
		int CraftAmount = (Crafting_Time_All - (Crafting_Time - PassTime)) / Crafting_Time_One;

		if (CraftAmount > Crafting_Time_All / Crafting_Time_One)
			CraftAmount = Crafting_Time_All / Crafting_Time_One;

		Crafting_Time -= PassTime;
		Crafting_Time_All -= CraftAmount * Crafting_Time_One;

		if (Result_Item == nullptr || (Result_Item->Data.ItemCode == 0 && Result_Item->Data.ItemAidCode))
		{
			GObjDataBase DataBase = MyGameInstance->ObjDataBase[CraftItemData.ItemCode][CraftItemData.ItemAidCode];

			CraftItemData.Amount = CraftAmount;
			CraftItemData.Attack = DataBase.Attack;
			CraftItemData.Distance = DataBase.Distance;
			CraftItemData.Health = DataBase.Health;
			CraftItemData.Sheild = DataBase.Sheild;

			Result_Item = CreateItemDataNode(CraftItemData);
		}

		else
			Result_Item->Data.Amount += CraftAmount;

		for (int a = 0; a < CraftAmount; a++)
		{
			GItemData Mater[3];
			for (auto Iter = Item_Material_Array.CreateConstIterator(); Iter; Iter++)
			{
				Mater[Iter.GetIndex()].ItemCode = (*Iter).ItemCode;
				Mater[Iter.GetIndex()].ItemAidCode = (*Iter).ItemAidCode;
				Mater[Iter.GetIndex()].Amount = (*Iter).Amount * CraftAmount;
			}

			for (int i = 0; i < 3; i++)
			{
				for (int m = 0; m < 3; m++)
				{
					if (Material_Item[m] != nullptr)
					{
						Material_Item[m]->Data.Amount -= Mater[i].Amount;

						if (Material_Item[m]->Data.Amount < 0)
						{
							Mater[i].Amount -= Material_Item[m]->Data.Amount;
							Material_Item[m]->Data.Amount = 0;
						}

						if (Material_Item[m]->Data.Amount == 0)
						{
							delete(Material_Item[m]);
							Material_Item[m] = nullptr;
						}

						Mater[i].Amount -= Mater[i].Amount;

						if (Mater[i].Amount <= 0)
							break;
					}

					else
						break;
				}
			}
		}
	}

	Parent->PointLight->LightColor = FColor::Red;

	Health = MyGameInstance->ObjDataBase[Data->Code][Data->AidCode].Health;
}

void Furnace::StartCrafting()
{
	UE_LOG(LogTemp, Warning, TEXT("StartCrafting"));
	//여기에 만들 수 있는 아이템을 검색하고 CraftItemData에 저장
	//타임 설정 후 만들기 시작.
	//Tick을 이용
	bFindCraftItem = false;
	Crafting_Time = 0;
	Crafting_Time_One = 0;
	Crafting_Time_All = 0;

	for (auto It = MyGameInstance->FurnaceDataBase.CreateConstIterator(); It; It++) //만들 수 있는 아이템 검색
	{
		for (int i = 0; i < 3; i++)
		{
			if (Material_Item[i] != nullptr)
			{
				Item_Material_Array.Empty();
				for (auto Iter = (*It).ResourceNeed.CreateConstIterator(); Iter; Iter++)
				{
					if (Material_Item[i]->Data.ItemCode != (*Iter).ItemCode ||
						Material_Item[i]->Data.ItemAidCode != (*Iter).ItemAidCode)
					{
						CraftItemData.ItemCode = 0;
						CraftItemData.ItemAidCode = 0;
						bFindCraftItem = false;
						break;
					}

					else if (Material_Item[i]->Data.Amount < (*Iter).Amount)
					{
						CraftItemData.ItemCode = 0;
						CraftItemData.ItemAidCode = 0;
						bFindCraftItem = false;
						break;
					}

					if (Crafting_Time > (*It).CraftingTime * (Material_Item[i]->Data.Amount / (*Iter).Amount))
						Crafting_Time = (*It).CraftingTime * (Material_Item[i]->Data.Amount / (*Iter).Amount);
					else if (Crafting_Time == 0)
						Crafting_Time = (*It).CraftingTime * (Material_Item[i]->Data.Amount / (*Iter).Amount);

					Crafting_Time_All = Crafting_Time;
					Crafting_Time_One = (*It).CraftingTime;
					CraftItemData.ItemCode = (*It).ItemCode;
					CraftItemData.ItemAidCode = (*It).ItemAidCode;
					bFindCraftItem = true;

					Item_Material_Array.Add((*Iter));
				}
			}
		}

		if (bFindCraftItem)
			break;
	}

	if (!bFindCraftItem) //만들 수 있는 아이템이 없으면 종료
		return;

	Data->Time = InGameLevel->GetPlayTime();

	Parent->SetActorTickEnabled(true);
}
