// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
#include "Engine.h"

UMyGameInstance::UMyGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	Plane = PlaneMesh.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial> ItemMater(TEXT("Material'/Game/ItemTexture/ItemMat.ItemMat'"));
	ItemMat = ItemMater.Object;

	static ConstructorHelpers::FObjectFinder<UTexture> Tex0(TEXT("/Game/Modeling/Tile/Texture/Water"));
	static ConstructorHelpers::FObjectFinder<UTexture> Tex1(TEXT("/Game/Modeling/Tile/Texture/Sand"));
	static ConstructorHelpers::FObjectFinder<UTexture> Tex2(TEXT("/Game/Modeling/Tile/Texture/Grass"));
	static ConstructorHelpers::FObjectFinder<UTexture> Tex3(TEXT("/Game/Modeling/Tile/Texture/Snow"));
	static ConstructorHelpers::FObjectFinder<UTexture> Tex4(TEXT("/Game/Modeling/Tile/Texture/Ice"));

	Texture[0] = Tex0.Object;
	Texture[1] = Tex1.Object;
	Texture[2] = Tex2.Object;
	Texture[3] = Tex3.Object;
	Texture[4] = Tex4.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh0(TEXT("/Game/Modeling/Tile/Tile"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh1(TEXT("/Game/Modeling/Object/Cloud/Cloud"));

	Mesh[0] = Mesh0.Object;
	Mesh[1] = Mesh1.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ObjMesh0_1_0(TEXT("/Game/Modeling/Object/Wood/Wood/Wood"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ObjMesh0_2_0(TEXT("/Game/Modeling/Object/Wood/Wood1/Wood1"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ObjMesh0_3_0(TEXT("/Game/Modeling/Object/Brazier/Furnace"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ObjMesh0_4_0(TEXT("StaticMesh'/Game/Modeling/Object/Stone/Stone.Stone'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ObjMesh2_1_0(TEXT("StaticMesh'/Game/Modeling/Item/PickAxe/Wood/WoodPickAxe.WoodPickAxe'"));
	
	ObjMesh[0][0][0] = nullptr;
	ObjMesh[0][1][0] = ObjMesh0_1_0.Object;
	ObjMesh[0][1][1] = ObjMesh0_1_0.Object;
	ObjMesh[0][2][0] = ObjMesh0_2_0.Object;
	ObjMesh[0][2][1] = ObjMesh0_2_0.Object;
	ObjMesh[0][3][0] = ObjMesh0_3_0.Object;
	ObjMesh[0][4][0] = ObjMesh0_4_0.Object;
	ObjMesh[2][1][0] = ObjMesh2_1_0.Object;

	static ConstructorHelpers::FObjectFinder<UTexture> ObjTex0_1_0(TEXT("/Game/Modeling/Object/Wood/Wood/Wood_Tex"));
	static ConstructorHelpers::FObjectFinder<UTexture> ObjTex0_2_0(TEXT("/Game/Modeling/Object/Wood/Wood1/Wood1_Tex"));
	static ConstructorHelpers::FObjectFinder<UTexture> ObjTex0_3_0(TEXT("/Game/Modeling/Object/Brazier/Furnace_Tex"));
	static ConstructorHelpers::FObjectFinder<UTexture> ObjTex0_4_0(TEXT("/Game/Modeling/Object/Stone/Stone_Tex"));
	static ConstructorHelpers::FObjectFinder<UTexture> ObjTex1_1_0(TEXT("Texture2D'/Game/ItemTexture/Log.Log'"));
	static ConstructorHelpers::FObjectFinder<UTexture> ObjTex1_2_0(TEXT("Texture2D'/Game/ItemTexture/Furnace_Item_Tex.Furnace_Item_Tex'"));
	static ConstructorHelpers::FObjectFinder<UTexture> ObjTex1_3_0(TEXT("Texture2D'/Game/ItemTexture/Stone_Item_Tex.Stone_Item_Tex'"));
	static ConstructorHelpers::FObjectFinder<UTexture> ObjTex1_4_0(TEXT("Texture2D'/Game/ItemTexture/Hot_Stone_Item_Tex.Hot_Stone_Item_Tex'"));
	static ConstructorHelpers::FObjectFinder<UTexture> ObjTex2_1_0(TEXT("Texture2D'/Game/Modeling/Item/PickAxe/Wood/WoodPickAxe_Tex.WoodPickAxe_Tex'"));
	static ConstructorHelpers::FObjectFinder<UTexture> ObjTex2_2_0(TEXT("Texture2D'/Game/ItemTexture/Wood_Body.Wood_Body'"));
		
	ObjTexture[0][0][0] = nullptr;
	ObjTexture[0][1][0] = ObjTex0_1_0.Object;
	ObjTexture[0][1][1] = ObjTex0_1_0.Object;
	ObjTexture[0][2][0] = ObjTex0_2_0.Object;
	ObjTexture[0][2][1] = ObjTex0_2_0.Object;
	ObjTexture[0][3][0] = ObjTex0_3_0.Object;
	ObjTexture[0][4][0] = ObjTex0_4_0.Object;
	ObjTexture[1][1][0] = ObjTex1_1_0.Object;
	ObjTexture[1][2][0] = ObjTex1_2_0.Object;
	ObjTexture[1][3][0] = ObjTex1_3_0.Object;
	ObjTexture[1][4][0] = ObjTex1_4_0.Object;
	ObjTexture[2][1][0] = ObjTex2_1_0.Object;
	ObjTexture[2][2][0] = ObjTex2_2_0.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial> ObjMat_Fade(TEXT("/Game/Modeling/Object/Wood/Fade_Mat"));
	static ConstructorHelpers::FObjectFinder<UMaterial> ObjMat_Wood(TEXT("/Game/Modeling/Object/Wood/Wood_Mat"));

	Mat[0] = ObjMat_Fade.Object;
	Mat[1] = ObjMat_Wood.Object;

	static ConstructorHelpers::FObjectFinder<UClass> AnimalAnim_1(TEXT("AnimBlueprint'/Game/Modeling/Animal/Pig/Animation/Pig_Animation_BluePrint.Pig_Animation_BluePrint_C'"));
	AnimalAnim[1] = AnimalAnim_1.Object;

	static ConstructorHelpers::FObjectFinder<UTexture> AnimalTex_1(TEXT("Texture2D'/Game/Modeling/Animal/Pig/Pig_Texture.Pig_Texture'"));
	AnimalTex[1] = AnimalTex_1.Object;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> AnimalMesh_1(TEXT("SkeletalMesh'/Game/Modeling/Animal/Pig/Pig_Rig.Pig_Rig'"));
	AnimalMesh[1] = AnimalMesh_1.Object;
}

void UMyGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, TEXT("Test"));
}