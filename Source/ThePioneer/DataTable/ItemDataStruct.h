// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemDataStruct.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FItemCode
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

UCLASS()
class THEPIONEER_API AItemDataStruct : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemDataStruct();
};
