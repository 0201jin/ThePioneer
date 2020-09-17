// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CraftingStruct.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FItemMaterial
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ItemCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ItemAidCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Amount;
};

USTRUCT(Atomic, BlueprintType)
struct FCraftItemList
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ItemCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ItemAidCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CanMake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture * Texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FItemMaterial> ResourceNeed;
};

UCLASS()
class THEPIONEER_API ACraftingStruct : public AActor
{
	GENERATED_BODY()
public:
	ACraftingStruct();
};