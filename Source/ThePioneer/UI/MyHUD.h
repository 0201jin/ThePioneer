// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UI/Inventory/InventoryWidget.h"
#include "UI/Furnace/FurnaceWidget.h"

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

using namespace MapStruct;
/**
 * 
 */
UCLASS()
class THEPIONEER_API AMyHUD : public AHUD
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		void Furnace_SwapItem(int _Number, int _NewNumber);

public:
	AMyHUD();

	void CloseAllWidget();

	void SwitchInventory();
	void SetInven(GItemDataNode*(*_Inven)[INVEN_ROW]);
	void SetInventory(GItemDataNode*(*_Inven)[INVEN_ROW]);
	void SetWorkBench();

	void SwitchFurnace();
	void SetFurnace(ObjectDataClass * _FurnaceAdr);

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

protected:
	class UClass * InvenWidgetClass;
	class UInventoryWidget * InventoryWidget;

	class UClass * FurnaceWidgetClass;
	class UFurnaceWidget *  FurnaceWidget;

	bool InvenSwitch;
	bool FurnaceSwitch;

	UMyGameInstance * MyGameInstance;

	GItemDataNode * Inven[INVEN_COL][INVEN_ROW];

	int ItemMaterial[4][256] = { 0 };

	TArray<FCraftingTableRow> CraftDataBase;
	TArray<FCraftItemList> CraftItemList;
};
