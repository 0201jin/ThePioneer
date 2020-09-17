// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemActor.h"

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetRelativeScale3D(FVector(1, 1, 1));
	Mesh->SetRelativeRotation(FRotator(0, 90, 40));

	Collision = NewObject<UBoxComponent>(this, TEXT("CollisionBox"));
	Collision->SetupAttachment(RootComponent);
	Collision->SetRelativeLocation(FVector(0, 0, 0));
	Collision->SetRelativeRotation(FRotator(0, 90, 40));
	Collision->InitBoxExtent(FVector(50, 50, 50));
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnOverlapBegin);

	QueueNum = 0;
}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemActor::SetMesh(UStaticMesh * _mesh)
{
	Mesh->SetStaticMesh(_mesh);
}

void AItemActor::SetData(GItemData _data)
{
	Data = _data;
}

void AItemActor::SetMat(UMaterial * _Mat)
{
	Instance = UMaterialInstanceDynamic::Create(_Mat, this);
	Mesh->SetMaterial(0, Instance);
}

void AItemActor::SetTex(UTexture * _tex)
{
	Instance->SetTextureParameterValue(TEXT("Texture"), _tex);
	Mesh->SetMaterial(0, Instance);
	bOverlap = true;
}

void AItemActor::SetQueueNumber(int _QueueNum)
{
	QueueNum = _QueueNum;
}

void AItemActor::SetInGameLevel(AInGameLevel * _InGameLevel)
{
	InGameLevel = _InGameLevel;
}

void AItemActor::SetNode(GItemDataNode * _node)
{
	Node = _node;
}

void AItemActor::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<APawn>(OtherActor) && bOverlap)
	{
		InGameLevel->LootingItem(GetActorLocation().X * 0.01, GetActorLocation().Y * 0.01, QueueNum, Node, Cast<APawn>(OtherActor));
		//충돌 박스를 만들고 레벨 스크립트에 HideActor를 만들어서 ItemMgr에 전달(QueueNum 인자 전달)
		//ItemMgr에서 queue 관리
	}
}

