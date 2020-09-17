// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Animal/AnimalDataClass.h"
#include "Animal/AnimalClass.h"

#include "CoreMinimal.h"

class THEPIONEER_API AnimalActor : public AnimalDataClass
{
public:	
	// Sets default values for this actor's properties
	AnimalActor();
	~AnimalActor();

public:
	void Tick(float DeltaTime, int x, int y);
	void SetData(GAnimalDataNode * _Data);

	void StartUpdate();
	void SetInGameLevel(AInGameLevel * _InGameLevel);

	void SetParent(AActor * _parent);
	void Distroyed();

	void Damage(int _Damage);
	void InitHealth(int _Health, int _WalkSpeed, int _RunSpeed);

	GAnimalDataNode * GetData();
protected:
	AInGameLevel * InGameLevel;
	AAnimalClass * Parent;

	GAnimalDataNode * Data;

	int x, y = 0;

	int Health = 100;
	int WalkSpeed = 0;
	int RunSpeed = 0;
	float FightModeTime = 0;
	float MoveTime = 10;
	float StayTime = 10;

	bool bFightMode = false;
	bool TickRate = true;
};
