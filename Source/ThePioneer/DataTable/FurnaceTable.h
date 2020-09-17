// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CraftingStruct.h"


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "FurnaceTable.generated.h"

USTRUCT(BlueprintType)
struct FFurnaceTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 ItemCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 ItemAidCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 CraftingTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		TArray<FItemMaterial> ResourceNeed;
};

UCLASS()
class THEPIONEER_API AFurnaceTable : public AActor
{
	GENERATED_BODY()
};
