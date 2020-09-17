// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "System/MapStruct.h"
#include "LevelScript/InGameLevel.h"
#include "Object/ObjectDataClass.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FurnaceWidget.generated.h"

/**
 * 
 */
using namespace MapStruct;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOpenFurnace);

UCLASS()
class THEPIONEER_API UFurnaceWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		UTexture * ButtonNumber(int _Number);

	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		int ItemAmount(int _Number);

	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		bool GetWear(int _Number);

	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		UTexture * CheckFurnace(int _Number);

	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		int CheckFurnaceAmount(int _Number);

	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		void StartCrafting();

	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		void TickA();

public:
	void FurnaceWidget();
	void OpenFurnace();

	void SetInventory(GItemDataNode*(*_Inven)[INVEN_ROW]);
	void SetFurnace(ObjectDataClass * _FurnaceAdr);

	void SwapInven(int _Number, int _NewNumber);

protected:
	GItemDataNode * Inven[INVEN_COL][INVEN_ROW];
	UMyGameInstance * MyGameInstance;

	ObjectDataClass * FurnaceAdr;

protected:
	UPROPERTY(BluePrintAssignable, Category = "UMG Function")
		FOpenFurnace FurnaceOpen;
};
