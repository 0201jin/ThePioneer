// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ItemDataStruct.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "AnimalTable.generated.h"

USTRUCT(BlueprintType)
struct FAnimalTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 Code;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		TArray<int32> Habitat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		TArray<FItemCode> DestItemCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		TArray<FItemCode> PriceDestItemCode;
};

UCLASS()
class THEPIONEER_API AAnimalTable : public AActor
{
	GENERATED_BODY()
};
