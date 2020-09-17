// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "System/MapStruct.h"
#include "LevelScript/InGameLevel.h"
#include "DataTable/CraftingTable.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 *
 */
using namespace MapStruct;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOpenInven);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOpenWorkBench, const TArray<FCraftItemList>&, ItemList);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCloseWorkBench);

UCLASS()
class THEPIONEER_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		UTexture * ButtonNumber(int _Number);

	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		int ItemAmount(int _Number);

	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		UTexture * GetTexture(int _ItemCode, int _ItemAidCode);

	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		int GetMatAmount(int _ItemCode, int _ItemAidCode);

	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		void CraftItem(TArray<FItemMaterial> _MaterList, FItemMaterial _CraftItem);

	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		UTexture * GetHeadTexture();

	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		UTexture * GetBodyTexture();

	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		UTexture * GetShooseTexture();

	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		bool GetWear(int _Number);

public:
	void InventoryWidget();
	void OpenInven();

	void WorkBenchWidget();
	void OpenWorkBench(TArray<FCraftItemList> _CraftList);
	void CloseWorkBench();

	void SetInventory(GItemDataNode*(*_Inven)[INVEN_ROW]);
	void SetItemMaterial(int(*_ItemMaterial)[256]);

protected:
	int ItemMaterial[4][256] = { 0 };
	TArray<FCraftItemList> CraftList;
	TArray<FCraftingTableRow> CraftDataBase;
	
	GItemDataNode * Inven[INVEN_COL][INVEN_ROW];
	UMyGameInstance * MyGameInstance;

protected:
	UPROPERTY(BluePrintAssignable, Category = "UMG Function")
		FOpenWorkBench WorkBenchOpen;

	UPROPERTY(BluePrintAssignable, Category = "UMG Function")
		FCloseWorkBench WorkBenchClose;

	UPROPERTY(BluePrintAssignable, Category = "UMG Function")
		FOpenInven InvenOpen;
};
