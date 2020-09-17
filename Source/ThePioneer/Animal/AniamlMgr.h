// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LevelScript/InGameLevel.h"
#include "System/MapStruct.h"
#include "Animal/AnimalClass.h"

#include "CoreMinimal.h"

#define ANIMALACTOR_COUNT 1000

using namespace MapStruct;
/**
 * 
 */
class THEPIONEER_API AniamlMgr
{
public:
	AniamlMgr(AInGameLevel * _InGameLevel);
	~AniamlMgr();

	void SpawnAnimalActor(UWorld * _world);

	void ArrayAnimalActor(int _x, int _y, int _cx, int _cy, GAnimalDataNode * _data);

	void SetAnimalChunkData(GFieldAnimalList(*_AnimalData)[3]);

	void HideAnimalChunkActor(int _x, int _y, int _lo);
	void ArrayAnimalChunkActor(int _x, int _y, int _lo);

protected:
	AAnimalClass * AnimalActorArr[ANIMALACTOR_COUNT];

	AInGameLevel * InGameLevel;
	UMyGameInstance * MyGameInstance;

	GItemSpawnQueue * AnimalCountQueue;
	GItemSpawnQueue * ActiveAnimalCountQueue[32][32][4];

	GFieldAnimalList(*AnimalData)[3]; 
};
