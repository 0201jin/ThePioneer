// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "System/MapStruct.h"
#include "DataTable/ItemDataTable.h"
#include "DataTable/CraftingTable.h"
#include "DataTable/FurnaceTable.h"
#include "DataTable/AnimalTable.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

#define TEXTURE_SIZE 5

#define MESH_SIZE 10

#define OBJ_MESH_FIRST 32
#define OBJ_MESH_SECOND 32
#define OBJ_MESH_THIRD 4

#define OBJ_TEXTURE_FIRST 32
#define OBJ_TEXTURE_SECOND 32
#define OBJ_TEXTURE_THIRD 4

#define MAT_SIZE 10

using namespace MapStruct;
/**
 * 
 */
UCLASS()
class THEPIONEER_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMyGameInstance();
	
protected:
	virtual void Init();

public:
	GObjDataBase ObjDataBase[256][256];
	FName ObjName[256][256];
	TArray <FFurnaceTableRow> FurnaceDataBase;
	TArray<FCraftingTableRow> CraftDataBase;
	FAnimalTableRow AnimalDataBase[256];

	UStaticMesh * Plane;
	UMaterial * ItemMat;

	char Habitat[5][256] = { 0 };
	char HabitatCount[5] = { 0 };

	UClass * AnimalAnim[256];
	UTexture * AnimalTex[256];
	USkeletalMesh * AnimalMesh[256];

	UTexture * Texture[TEXTURE_SIZE];
	UStaticMesh * Mesh[MESH_SIZE];
	UStaticMesh * ObjMesh[OBJ_MESH_FIRST][OBJ_MESH_SECOND][OBJ_MESH_THIRD] = { (nullptr), (nullptr) };
	UTexture * ObjTexture[OBJ_TEXTURE_FIRST][OBJ_TEXTURE_SECOND][OBJ_TEXTURE_THIRD];
	UMaterial * Mat[MAT_SIZE];
};
