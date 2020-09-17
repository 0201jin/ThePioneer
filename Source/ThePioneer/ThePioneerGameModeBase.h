// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MyGameInstance.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ThePioneerGameModeBase.generated.h"

using namespace MapStruct;

/**
 *
 */
UCLASS()
class THEPIONEER_API AThePioneerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AThePioneerGameModeBase();
	virtual void BeginPlay() override;

protected:
	class UDataTable * ItemDataTable;
	class UDataTable * CraftingTable;
	class UDataTable * FurnaceTable;
	class UDataTable * AnimalTable;

	UMyGameInstance * MyGameInstance;
};
