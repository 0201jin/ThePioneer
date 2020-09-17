// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LevelScript/InGameLevel.h"

#include "CoreMinimal.h"

/**
 * 
 */
class THEPIONEER_API AnimalDataClass
{
public:
	AnimalDataClass();
	virtual ~AnimalDataClass();

public:
	virtual void Tick(float DeltaTime, int x, int y);
	virtual void SetData(GAnimalDataNode * _Data);

	virtual void StartUpdate();
	virtual void SetInGameLevel(AInGameLevel * _InGameLevel);

	virtual void SetParent(AActor * parent);
	virtual void Distroyed();

	virtual void Damage(int _Damage);
	virtual void InitHealth(int _Health, int _WalkSpeed, int _RunSpeed);

	virtual GAnimalDataNode * GetData();

	UMyGameInstance * MyGameInstance;
};
