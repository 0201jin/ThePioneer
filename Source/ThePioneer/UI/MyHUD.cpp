// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHUD.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Engine.h"

AMyHUD::AMyHUD()
{
	static ConstructorHelpers::FClassFinder<UInventoryWidget> InventoryUIClass(TEXT("WidgetBlueprint'/Game/UI/Inventory/Inventory_BP'"));
	InvenWidgetClass = InventoryUIClass.Class;

	static ConstructorHelpers::FClassFinder<UFurnaceWidget> FurnaceUIClass(TEXT("WidgetBlueprint'/Game/UI/Furnace/Furnace_BP'"));
	FurnaceWidgetClass = FurnaceUIClass.Class;
}

void AMyHUD::CloseAllWidget()
{
	InvenSwitch = false;
	FurnaceSwitch = false;
	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	FurnaceWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AMyHUD::Furnace_SwapItem(int _Number, int _NewNumber)
{
	FurnaceWidget->SwapInven(_Number, _NewNumber);
}

void AMyHUD::SwitchInventory()
{
	if (InvenSwitch)
	{
		InventoryWidget->CloseWorkBench();
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		InvenSwitch = false;
		return;
	}

	CloseAllWidget();
	SetWorkBench();
	InventoryWidget->SetInventory(Inven);
	InventoryWidget->OpenWorkBench(CraftItemList);
	InventoryWidget->OpenInven();
	InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	InvenSwitch = true;
}

void AMyHUD::SwitchFurnace()
{
	if (FurnaceSwitch)
	{
		FurnaceSwitch = false;
		FurnaceWidget->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	CloseAllWidget();
	FurnaceWidget->SetInventory(Inven);
	FurnaceWidget->OpenFurnace();
	FurnaceWidget->SetVisibility(ESlateVisibility::Visible);
	FurnaceSwitch = true;
}

void AMyHUD::SetFurnace(ObjectDataClass * _FurnaceAdr)
{
	FurnaceWidget->SetFurnace(_FurnaceAdr);
}

void AMyHUD::SetInven(GItemDataNode *(*_Inven)[INVEN_ROW])
{
	SetInventory(_Inven);

	if (InvenSwitch)
	{
		InventoryWidget->SetInventory(Inven);
		SetWorkBench();
		InventoryWidget->OpenWorkBench(CraftItemList);
		InventoryWidget->OpenInven();
	}

	else if (FurnaceSwitch)
	{
		FurnaceWidget->SetInventory(Inven);
		FurnaceWidget->OpenFurnace();
	}
}

void AMyHUD::SetInventory(GItemDataNode *(*_Inven)[INVEN_ROW])
{
	for (int i = 0; i < INVEN_SIZE; i++)
		*(*Inven + i) = *(*_Inven + i);
}

void AMyHUD::SetWorkBench()
{
	for (int i = 0; i < 1024; i++)
	{
		(*(*ItemMaterial + i)) = 0;
	}

	CraftItemList.Empty();

	for (int i = 0; i < INVEN_SIZE; i++)
	{
		if (*(*Inven + i) != nullptr)
		{
			ItemMaterial[(*(*Inven + i))->Data.ItemCode][(*(*Inven + i))->Data.ItemAidCode] += (*(*Inven + i))->Data.Amount;
			UE_LOG(LogTemp, Warning, TEXT("Inven %d"), sizeof(ItemMaterial) / sizeof(ItemMaterial[0][0]));
		}
	}
	//여기에 CraftBase와 Inven을 계산 후 만들어진것과 안만들어진 것을 CraftItemList에 추가
	//CraftItemList는 WorkBenchWidget클래스로 이동

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

		CraftItemList.Add(CraftData);
	}

	InventoryWidget->SetItemMaterial(ItemMaterial);
}

void AMyHUD::BeginPlay()
{
	MyGameInstance = Cast<UMyGameInstance>(GetGameInstance());

	GetOwningPlayerController()->SetInputMode(FInputModeGameAndUI());

	InventoryWidget = CreateWidget<UInventoryWidget>(GetOwningPlayerController(), InvenWidgetClass);
	InventoryWidget->AddToViewport();
	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	InventoryWidget->InventoryWidget();

	FurnaceWidget = CreateWidget<UFurnaceWidget>(GetOwningPlayerController(), FurnaceWidgetClass);
	FurnaceWidget->AddToViewport();
	FurnaceWidget->SetVisibility(ESlateVisibility::Hidden);
	FurnaceWidget->FurnaceWidget();

	for (auto It = MyGameInstance->CraftDataBase.CreateConstIterator(); It; It++)
	{
		CraftDataBase.Add(*It);
	}
}

void AMyHUD::Tick(float DeltaTime)
{
}
