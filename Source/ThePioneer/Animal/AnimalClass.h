// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LevelScript/InGameLevel.h"
#include "Animal/AnimalDataClass.h"
#include "Components/BoxComponent.h"
#include "UI/MyHUD.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AnimalClass.generated.h"

using namespace MapStruct;
UCLASS()
class THEPIONEER_API AAnimalClass : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent * SkeletalMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UBoxComponent * CameraCollision;

public:	
	// Sets default values for this actor's properties
	AAnimalClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetMat(UTexture * tex, UMaterial * _Mat);
	void SetMesh(USkeletalMesh * _mesh, UClass * _Anim);

	void SetChild(char _Code, GAnimalDataNode * _Data);
	void SetSize(char _Code);

	//Child와 연결되는 함수
	void SetData(GAnimalDataNode * _Data);
	void StartUpdate();

	void SetInGameLevel(AInGameLevel * _InGameLevel);

	void Damage(int _Damage);

protected:
	AnimalDataClass * Child;
	AInGameLevel * InGameLevel;

	UMaterialInterface * Mater;
	UMaterialInstanceDynamic * Instance;
	UMaterial * Mat;
	UTexture * Texture;
};
