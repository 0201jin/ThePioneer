// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectActor.h"

ObjectActor::ObjectActor()
{
}

ObjectActor::~ObjectActor()
{
}

void ObjectActor::Tick(float DeltaTime, int _x, int _y)
{
	if (Data->Grow < MyGameInstance->ObjDataBase[Data->Code][Data->AidCode].Obj_Max_Grow)
	{
		Data->FExp += 1 + DeltaTime;

		if (Data->FExp >= 255)
		{
			Data->Exp++;
			Data->FExp = 0;
		}

		if (Data->Exp >= 255)
		{
			Data->Grow++;
			Data->Exp = 0;

			x = _x * 0.01;
			y = _y * 0.01;

			InGameLevel->ChangeObjChunk(x, y, *Data);
		}

		Data->Time = InGameLevel->GetPlayTime();
	}
	else
	{
		Parent->SetActorTickEnabled(false);
	}
}

void ObjectActor::SetData(GObjectData * _Data)
{
	Data = _Data;
}

void ObjectActor::StartUpdate()
{
	float WorldPlayTime = InGameLevel->GetPlayTime();
	int AWayTime = WorldPlayTime - Data->Time;

	if (Data->Grow < MyGameInstance->ObjDataBase[Data->Code][Data->AidCode].Obj_Max_Grow)
	{
		int FExp = (AWayTime + 1) % 257;
		int Exp = AWayTime / 256;
		int Grow = Exp / 256;
		Exp = (Grow + 1) % 257;

		Data->FExp = FExp;
		Data->Exp = Exp;
		Data->Grow = Grow;
	}
}

void ObjectActor::SetInGameLevel(AInGameLevel * _InGameLevel)
{
	InGameLevel = _InGameLevel;
	MyGameInstance = _InGameLevel->MyGameInstance;
}

void ObjectActor::SetParent(AActor * _parent)
{
	Parent = Cast<AObjectClass>(_parent);
}

void ObjectActor::Distroyed()
{
	Parent->SetActorTickEnabled(false);
	Parent->SetActorHiddenInGame(true);
}

void ObjectActor::Damage(int _Damage)
{
	Health -= _Damage;

	if (Health <= 0)
	{
		InGameLevel->DeleteObjChunk(Parent->GetActorLocation().X * 0.01, Parent->GetActorLocation().Y * 0.01);
	}
}

void ObjectActor::InitHealth()
{
	Health = MyGameInstance->ObjDataBase[Data->Code][Data->AidCode].Health;
}

GObjectData * ObjectActor::GetData()
{
	return Data;
}

bool ObjectActor::GetActive()
{
	return false;
}
