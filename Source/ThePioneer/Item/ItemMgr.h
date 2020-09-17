// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LevelScript/InGameLevel.h"
#include "System/MapStruct.h"
#include "ItemActor.h"

#include "CoreMinimal.h"

#define ITEMACTOR_COUNT 1000

using namespace MapStruct;
/**
 * 
 */
class THEPIONEER_API ItemMgr
{
public:
	ItemMgr(AInGameLevel * _InGameLevel);
	~ItemMgr();

	void SpawnItemActor(UWorld * _world);

	void ArrayItemActor(int _x, int _y, int _cx, int _cy, GItemData _data, GItemDataNode * _node);

	void HideItemChunkActor(int _x, int _y, int _lo);
	void ArrayItemChunkActor(int _x, int _y, int _lo);

	void LootingItem(int _x, int _y, int _lo, int _num);

	void SetItemChunkData(GFieldItemList(*_ItemData)[3]);
protected:
	AItemActor * ItemActorArr[ITEMACTOR_COUNT];

	GFieldItemList(*ItemData)[3];

	AInGameLevel * InGameLevel;

	GItemSpawnQueue * ItemCountQueue;

	GItemSpawnQueue * ActiveItemCountQueue[32][32][4];
};
