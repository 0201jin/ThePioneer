// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimalActor.h"

// Sets default values
AnimalActor::AnimalActor()
{
}

AnimalActor::~AnimalActor()
{
}

void AnimalActor::Tick(float DeltaTime, int x, int y)
{
	MoveTime -= DeltaTime;

	if (MoveTime > 0)
	{
		if (bFightMode)
		{
			//RunSpeed * DeltaTime;

			//이동하는 코드
			FVector Move = Parent->GetActorForwardVector() * (RunSpeed * DeltaTime);
			Parent->AddActorWorldOffset(Move);

			FightModeTime += DeltaTime;
			if (FightModeTime > 3)
				bFightMode = false;
		}
		else
		{		
			//WalkSpeed * DeltaTime;

			//이동하는 코드
			FVector Move = Parent->GetActorForwardVector() * (WalkSpeed * DeltaTime);
			Parent->AddActorWorldOffset(Move);
		}

		//이동 후 실시간 범위에서 벗어났는지 확인 후 데이터 조정.
	}

	else
	{
		StayTime -= DeltaTime;

		if (StayTime > 0)
		{
			StayTime = rand() % 10;
			MoveTime = rand() % 10;
			//방향 변경
			float RandYaw = (rand() % 3600) * 0.1;
			Parent->SetActorRotation(FRotator(0, RandYaw, 0));
		}
	}
}

void AnimalActor::SetData(GAnimalDataNode * _Data)
{
	Data = _Data;
}

void AnimalActor::StartUpdate()
{
}

void AnimalActor::SetInGameLevel(AInGameLevel * _InGameLevel)
{
	InGameLevel = _InGameLevel;
	MyGameInstance = _InGameLevel->MyGameInstance;
}

void AnimalActor::SetParent(AActor * _parent)
{
	Parent = Cast<AAnimalClass>(_parent);
}

void AnimalActor::Distroyed()
{
	Parent->SetActorTickEnabled(false);
	Parent->SetActorHiddenInGame(true);
}

void AnimalActor::Damage(int _Damage)
{
	bFightMode = true;
	FightModeTime = 0;

	Health -= _Damage;

	if (Health <= 0)
	{
		InGameLevel->DeleteObjChunk(Parent->GetActorLocation().X * 0.01, Parent->GetActorLocation().Y * 0.01);
	}
}

void AnimalActor::InitHealth(int _Health, int _WalkSpeed, int _RunSpeed)
{
	Health = _Health;
	WalkSpeed = _WalkSpeed;
	RunSpeed = _RunSpeed;
}

GAnimalDataNode * AnimalActor::GetData()
{
	return Data;
}
