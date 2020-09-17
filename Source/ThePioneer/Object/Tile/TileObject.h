// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Object/ObjectClass.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileObject.generated.h"

UCLASS()
class THEPIONEER_API ATileObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATileObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetMat(UTexture * tex);
	void SetMesh(UStaticMesh * _mesh);

protected:
	UStaticMeshComponent * Mesh;
	UMaterialInterface * Mater;
	UMaterialInstanceDynamic * Instance;
};
