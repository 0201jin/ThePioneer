// Fill out your copyright notice in the Description page of Project Settings.


#include "ThePioneerGameModeBase.h"
#include "Player/MyPlayerController.h"
#include "Player/PlayerPawn.h"
#include "UI/MyHUD.h"
#include "Engine.h"

AThePioneerGameModeBase::AThePioneerGameModeBase()
{
	DefaultPawnClass = APlayerPawn::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
	HUDClass = AMyHUD::StaticClass();

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemTable(TEXT("/Game/DataTable/ItemDataTable"));
	ItemDataTable = ItemTable.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> CraftingDataTable(TEXT("/Game/DataTable/CraftingTable"));
	CraftingTable = CraftingDataTable.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> FurnaceDataTable(TEXT("DataTable'/Game/DataTable/FurnaceTable.FurnaceTable'"));
	FurnaceTable = FurnaceDataTable.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> AnimalDataTable(TEXT("DataTable'/Game/DataTable/AnimalTable.AnimalTable'"));
	AnimalTable = AnimalDataTable.Object;
}

void AThePioneerGameModeBase::BeginPlay()
{
	FString ContextString;
	TArray<FName> RowNames;
	RowNames = ItemDataTable->GetRowNames();

	MyGameInstance = Cast<UMyGameInstance>(GetGameInstance());

	for (auto& name : RowNames)
	{
		FItemDataTableRow * ItemDataTableRow = ItemDataTable->FindRow<FItemDataTableRow>(name, ContextString);
		if (ItemDataTableRow)
		{
			GObjDataBase Data;
			Data.ItemCode = ItemDataTableRow->ItemCode;
			Data.ItemAidCode = ItemDataTableRow->ItemAidCode;
			Data.DisposeItemCode = ItemDataTableRow->DisPoseItemCode;
			Data.DisposeItemAidCode = ItemDataTableRow->DisPoseItemAidCode;

			Data.Obj_Max_Grow = ItemDataTableRow->Max_Grow;
			Data.Obj_Mat_Num = ItemDataTableRow->Mat_Num;
			Data.Can_Dispose = ItemDataTableRow->Can_Dispose;
			Data.Can_Install[0] = ItemDataTableRow->Can_Water;

			Data.Can_Install[1] = ItemDataTableRow->Can_Sand;
			Data.Can_Install[2] = ItemDataTableRow->Can_Grass;
			Data.Can_Install[3] = ItemDataTableRow->Can_Snow;
			Data.Can_Install[4] = ItemDataTableRow->Can_Ice;

			Data.DestItemCode = ItemDataTableRow->DestItemCode;
			Data.PriceDestItemCode = ItemDataTableRow->PriceDestItemCode;
			Data.Used = ItemDataTableRow->Used;
			Data.Attack = ItemDataTableRow->Attack;

			Data.Can_Wear = ItemDataTableRow->Can_Wear;
			Data.Parts = ItemDataTableRow->Parts;
			Data.Sheild = ItemDataTableRow->Sheild;
			Data.Distance = ItemDataTableRow->Distance;
			Data.Health = ItemDataTableRow->Health;

			Data.Amount = ItemDataTableRow->Amount;
			Data.Max_Amount = ItemDataTableRow->Max_Amount;

			MyGameInstance->ObjDataBase[Data.ItemCode][Data.ItemAidCode] = Data;
		}
	}

	FString CraftContextString;
	TArray<FName> CraftRowNames;
	CraftRowNames = CraftingTable->GetRowNames();

	for (auto& name : CraftRowNames)
	{
		FCraftingTableRow * CraftingTableRow = CraftingTable->FindRow<FCraftingTableRow>(name, CraftContextString);
		if (CraftingTableRow)
		{
			MyGameInstance->ObjName[CraftingTableRow->ItemCode][CraftingTableRow->ItemAidCode] = name;
			MyGameInstance->CraftDataBase.Add(*CraftingTableRow);
		}
	}

	FString FurnaceContextString;
	TArray<FName> FurnaceRowNames;
	FurnaceRowNames = FurnaceTable->GetRowNames();

	for (auto& name : FurnaceRowNames)
	{
		FFurnaceTableRow * FurnaceTableRow = FurnaceTable->FindRow<FFurnaceTableRow>(name, FurnaceContextString);
		if (FurnaceTableRow)
		{
			MyGameInstance->FurnaceDataBase.Add(*FurnaceTableRow);
		}
	}

	FString AnimalContextString;
	TArray<FName> AnimalRowNames;
	AnimalRowNames = AnimalTable->GetRowNames();

	for (auto& name : AnimalRowNames)
	{
		FAnimalTableRow * AnimalTableRow = AnimalTable->FindRow<FAnimalTableRow>(name, AnimalContextString);
		if (AnimalTableRow)
		{
			MyGameInstance->AnimalDataBase[(*AnimalTableRow).Code] = (*AnimalTableRow);

			for (auto It = (*AnimalTableRow).Habitat.CreateConstIterator(); It; It++)
			{
				MyGameInstance->Habitat[(*It)][MyGameInstance->HabitatCount[(*It)]] = (*AnimalTableRow).Code;
				MyGameInstance->HabitatCount[(*It)] += 1;
			}
		}
	}
}
