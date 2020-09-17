// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectClass.h"
#include "Engine.h"

#include "ActorObject/ObjectActor.h"
#include "Furnace/Furnace.h"

// Sets default values
AObjectClass::AObjectClass()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetRelativeScale3D(FVector(1, 1, 1));
	Mesh->SetRelativeRotation(FRotator(0, 0, 0));

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point"));
	PointLight->SetupAttachment(Mesh);
	PointLight->SetAttenuationRadius(0);
	PointLight->bHiddenInGame = false;
	PointLight->Activate(false);
}

// Called when the game starts or when spawned
void AObjectClass::BeginPlay()
{
	Super::BeginPlay();

	InGameLevel = Cast<AInGameLevel>(GetWorld()->GetLevelScriptActor());
}

// Called every frame
void AObjectClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Child != nullptr)
		Child->Tick(DeltaTime, GetActorLocation().X, GetActorLocation().Y);
}

void AObjectClass::SetMat(UTexture * tex, UMaterial * _Mat)
{
	Texture = tex;
	Mat = _Mat;
	Instance = UMaterialInstanceDynamic::Create(_Mat, this);
	Instance->SetTextureParameterValue(TEXT("Tex"), tex);
	Mesh->SetMaterial(0, Instance);
}

void AObjectClass::SetMesh(UStaticMesh * _mesh)
{
	Mesh->SetStaticMesh(_mesh);
	//Instance = UMaterialInstanceDynamic::Create(Mesh->GetMaterial(0), this);
}

void AObjectClass::SetFadeMat(UMaterial * _Mat_Fade)
{
	Mat_Fade = _Mat_Fade;
}

void AObjectClass::FadeIn()
{
	Instance = UMaterialInstanceDynamic::Create(Mat_Fade, this);
	Instance->SetTextureParameterValue(TEXT("Tex"), Texture);
	Mesh->SetMaterial(0, Instance);
}

void AObjectClass::FadeOut()
{
	Instance = UMaterialInstanceDynamic::Create(Mat, this);
	Instance->SetTextureParameterValue(TEXT("Tex"), Texture);
	Mesh->SetMaterial(0, Instance);
}

void AObjectClass::SetChild(char _ItemCode, char _ItemAidCode, GObjectData * _Data)
{
	if (Child != nullptr)
		if (!Child->GetActive())
			delete(Child);

	switch (_ItemCode)
	{
	case 0:
		switch (_ItemAidCode)
		{
		case 3:
			if ((*_Data).ObjAdr == nullptr || (*_Data).ObjAdr == 0)
				Child = new Furnace;
			else
				Child = (ObjectDataClass*)(*_Data).ObjAdr;

			(*_Data).ObjAdr = (void*)Child;

			Child->SetParent(this);
			Child->SetInGameLevel(InGameLevel);
			break;

		default:
			Child = new ObjectActor;
			Child->SetParent(this);

			SetActorTickEnabled(true);
			SetActorScale3D(FVector(1, 1, 1));

			Child->SetInGameLevel(InGameLevel);
			break;
		}
		break;
	}
}

void AObjectClass::SetSize(char _ItemCode, char _ItemAidCode)
{
	switch (_ItemCode)
	{
	case 0:
		switch (_ItemAidCode)
		{
		case 3:
			SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, 155));
			SetActorScale3D(FVector(0.25, 0.25, 0.25));
			SetActorRotation(FRotator(0, 90, 0));
			break;

		case 4:
			SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, 145));
			SetActorScale3D(FVector(0.5, 0.5, 0.5));
			SetActorRotation(FRotator(0, 90 * (rand() % 4), 0));
			break;

		default:
			break;
		}
		break;
	}
}

void AObjectClass::SetData(GObjectData * _Data)
{
	if (Child != nullptr && _Data != nullptr)
	{
		Child->SetData(_Data);
		Child->InitHealth();
	}
}

void AObjectClass::StartUpdate()
{
	if (Child != nullptr)
		Child->StartUpdate();
}

void AObjectClass::SetInGameLevel(AInGameLevel * _InGameLevel)
{
	if (Child != nullptr)
		Child->SetInGameLevel(InGameLevel);
}

void AObjectClass::Action(APlayerController * _Controller)
{
	if (this != nullptr && Child != nullptr && _Controller != nullptr)
		Child->Action(_Controller);
}

void AObjectClass::Damage(int _Damage)
{
	if (Child != nullptr)
		Child->Damage(_Damage);
}
