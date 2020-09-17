// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimalClass.h"
#include "Engine.h"

#include "Animal/AnimalActor.h"

// Sets default values
AAnimalClass::AAnimalClass()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	CameraCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CameraCollision;
	CameraCollision->SetRelativeLocation(FVector(0, 0, 0));
	CameraCollision->InitBoxExtent(FVector(50, 50, 50));
	//CameraCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerPawn::OnOverlapBegin);
	//CameraCollision->OnComponentEndOverlap.AddDynamic(this, &APlayerPawn::OnOverlapEnd);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(CameraCollision);
	SkeletalMesh->SetRelativeScale3D(FVector(1, 1, 1));
	SkeletalMesh->SetRelativeRotation(FRotator(0, 0, 0));
}

// Called when the game starts or when spawned
void AAnimalClass::BeginPlay()
{
	Super::BeginPlay();

	InGameLevel = Cast<AInGameLevel>(GetWorld()->GetLevelScriptActor());
}

// Called every frame
void AAnimalClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Child != nullptr)
		Child->Tick(DeltaTime, GetActorLocation().X, GetActorLocation().Y);
}

void AAnimalClass::SetInGameLevel(AInGameLevel * _InGameLevel)
{
	if (Child != nullptr)
		Child->SetInGameLevel(InGameLevel);
}

void AAnimalClass::SetMat(UTexture * _tex, UMaterial * _Mat)
{
	Texture = _tex;
	Mat = _Mat;
	Instance = UMaterialInstanceDynamic::Create(_Mat, nullptr);
	Instance->SetTextureParameterValue(TEXT("Tex"), _tex);
	SkeletalMesh->SetMaterial(0, Instance);
}

void AAnimalClass::SetMesh(USkeletalMesh * _mesh, UClass * _Anim)
{
	SkeletalMesh->SetSkeletalMesh(_mesh);
	SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SkeletalMesh->AnimClass = _Anim;
}

void AAnimalClass::SetChild(char _Code, GAnimalDataNode * _Data)
{
	if (Child != nullptr)
		delete(Child);

	switch (_Code)
	{
	case 0:
		break;

	default:
		Child = new AnimalActor;
		Child->SetParent(this);

		SetActorScale3D(FVector(1, 1, 1));

		Child->SetInGameLevel(InGameLevel);
		break;
	}

	SetActorTickEnabled(true);
}

void AAnimalClass::SetSize(char _Code)
{
	switch (_Code)
	{
	case 1:
		SkeletalMesh->SetRelativeRotation(FRotator(0, -90, 0));
		CameraCollision->SetBoxExtent(FVector(70, 130, 50));
		break;

	default:
		break;
	}
}

void AAnimalClass::SetData(GAnimalDataNode * _Data)
{
	if (Child != nullptr && _Data != nullptr)
	{
		int Code = _Data->Data.AnimalCode;
		Child->SetData(_Data);
		Child->InitHealth(InGameLevel->MyGameInstance->AnimalDataBase[Code].Health,
			InGameLevel->MyGameInstance->AnimalDataBase[Code].WalkSpeed,
			InGameLevel->MyGameInstance->AnimalDataBase[Code].RunSpeed);
	}
}

void AAnimalClass::StartUpdate()
{
	if (Child != nullptr)
		Child->StartUpdate();
}

void AAnimalClass::Damage(int _Damage)
{
	if (Child != nullptr)
		Child->Damage(_Damage);
}

