// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LevelScript/InGameLevel.h"
#include "Object/ObjectDataClass.h"
#include "Components/PointLightComponent.h"
#include "UI/MyHUD.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectClass.generated.h"

using namespace MapStruct;
UCLASS()
class THEPIONEER_API AObjectClass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetMat(UTexture * tex, UMaterial * _Mat);
	void SetMesh(UStaticMesh * _mesh);
	void SetFadeMat(UMaterial * _Mat_Fade);

	void FadeIn();
	void FadeOut();

	void SetChild(char _ItemCode, char _ItemAidCode, GObjectData * _Data);
	void SetSize(char _ItemCode, char _ItemAidCode);

	//Child와 연결되는 함수
	void SetData(GObjectData * _Data);
	void StartUpdate();

	void SetInGameLevel(AInGameLevel * _InGameLevel);
	void Action(APlayerController * _Controller);

	void Damage(int _Damage);

public:
	UStaticMeshComponent * Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UPointLightComponent * PointLight;

protected:
	ObjectDataClass * Child;

	AInGameLevel * InGameLevel;

	UMaterialInterface * Mater;
	UMaterialInstanceDynamic * Instance;
	UMaterial * Mat_Fade;
	UMaterial * Mat;
	UTexture * Texture;
};
