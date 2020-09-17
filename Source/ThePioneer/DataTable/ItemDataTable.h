// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ItemDataStruct.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "ItemDataTable.generated.h"

USTRUCT(BlueprintType)
struct FItemDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 ItemCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 ItemAidCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 Max_Grow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 Mat_Num;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		bool Can_Dispose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 DisPoseItemCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 DisPoseItemAidCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		bool Can_Water;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		bool Can_Sand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		bool Can_Grass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		bool Can_Snow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		bool Can_Ice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		TArray<FItemCode> DestItemCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		TArray<FItemCode> PriceDestItemCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 Used;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		bool Can_Wear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 Parts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 Sheild;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 Distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 Max_Amount;
};

UCLASS()
class THEPIONEER_API AItemDataTable : public AActor
{
	GENERATED_BODY()
};
