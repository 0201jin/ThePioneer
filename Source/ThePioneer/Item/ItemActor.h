// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "System/MapStruct.h"
#include "LevelScript/InGameLevel.h"
#include "Components/BoxComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"

using namespace MapStruct;
UCLASS()
class THEPIONEER_API AItemActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UBoxComponent * Collision;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Sets default values for this actor's properties
	AItemActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetMesh(UStaticMesh * _mesh);
	void SetData(GItemData _data);
	void SetMat(UMaterial * _Mat);
	void SetTex(UTexture * _tex);
	void SetQueueNumber(int _QueueNum);
	void SetInGameLevel(AInGameLevel * _InGameLevel);
	void SetNode(GItemDataNode * _node);

public:
	bool bOverlap = true;

protected:
	UStaticMeshComponent * Mesh;
	UMaterialInstanceDynamic * Instance;
	GItemData Data;
	GItemDataNode * Node;

	AInGameLevel * InGameLevel;

	int QueueNum = 0;
};
