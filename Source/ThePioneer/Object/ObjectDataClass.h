// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LevelScript/InGameLevel.h"

#include "CoreMinimal.h"

/**
 * 
 */
class THEPIONEER_API ObjectDataClass
{
public:
	ObjectDataClass();
	virtual ~ObjectDataClass();

public:
	virtual void Tick(float DeltaTime, int x, int y);
	virtual void SetData(GObjectData * _Data);

	virtual void StartUpdate();
	virtual void SetInGameLevel(AInGameLevel * _InGameLevel);

	virtual void SetParent(AActor * parent);

	virtual void Distroyed();
	virtual void Action(APlayerController * _Controller);

	virtual void Damage(int _Damage);
	virtual void InitHealth();

	virtual bool GetActive();
	virtual GObjectData * GetData();

	UMyGameInstance * MyGameInstance;
};