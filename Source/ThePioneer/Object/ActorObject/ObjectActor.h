// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Object/ObjectDataClass.h"
#include "Object/ObjectClass.h"

#include "CoreMinimal.h"

/**
 * 
 */
class THEPIONEER_API ObjectActor : public ObjectDataClass
{
public:
	ObjectActor();
	~ObjectActor();

public:
	void Tick(float DeltaTime, int x, int y);
	void SetData(GObjectData * _Data);

	void StartUpdate();
	void SetInGameLevel(AInGameLevel * _InGameLevel);

	void SetParent(AActor * _parent);

	void Distroyed();

	void Damage(int _Damage);
	void InitHealth();

	GObjectData * GetData();

	bool GetActive();
protected:
	AInGameLevel * InGameLevel;
	AObjectClass * Parent;

	GObjectData * Data;

	int x, y = 0;

	int Health = 100;
	bool TickRate = true;
};
