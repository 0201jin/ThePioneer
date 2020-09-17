// Fill out your copyright notice in the Description page of Project Settings.


#include "Cloud.h"

// Sets default values
ACloud::ACloud()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetRelativeScale3D(FVector(0.25, 0.25, 0.25));
	Mesh->SetRelativeRotation(FRotator(0, 0, 0));
}

// Called when the game starts or when spawned
void ACloud::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACloud::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACloud::SetMesh(UStaticMesh * _mesh)
{
	Mesh->SetStaticMesh(_mesh);
}

