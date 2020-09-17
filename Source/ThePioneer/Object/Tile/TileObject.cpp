// Fill out your copyright notice in the Description page of Project Settings.

#include "TileObject.h"
#include "Engine.h"

// Sets default values
ATileObject::ATileObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetRelativeScale3D(FVector(25, 25, 25));
	Mesh->SetRelativeRotation(FRotator(0, 0, 0));
}

// Called when the game starts or when spawned
void ATileObject::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATileObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATileObject::SetMat(UTexture * tex)
{
	Instance->SetTextureParameterValue(TEXT("Texture"), tex);
	Mesh->SetMaterial(0, Instance);
}

void ATileObject::SetMesh(UStaticMesh * _mesh)
{
	Mesh->SetStaticMesh(_mesh);
	Instance = UMaterialInstanceDynamic::Create(Mesh->GetMaterial(0), this);
}

