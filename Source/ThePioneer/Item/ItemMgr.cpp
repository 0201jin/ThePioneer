// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemMgr.h"
#include "Engine.h"

ItemMgr::ItemMgr(AInGameLevel * _InGameLevel)
{
	InGameLevel = _InGameLevel;

	ItemCountQueue = new GItemSpawnQueue;
	ItemSpawnQueueInit(ItemCountQueue);

	for (int x = 0; x < 32; x++)
		for (int y = 0; y < 32; y++)
			for (int i = 0; i < 4; i++)
			{
				ActiveItemCountQueue[x][y][i] = new GItemSpawnQueue;
				ItemSpawnQueueInit(ActiveItemCountQueue[x][y][i]);
			}
}

ItemMgr::~ItemMgr()
{
}

void ItemMgr::SpawnItemActor(UWorld * _world)
{
	UE_LOG(LogTemp, Warning, TEXT("ItemActor Spawn Start!"));

	UStaticMesh * Mesh = InGameLevel->MyGameInstance->Plane;
	UMaterial * ItemMat = InGameLevel->MyGameInstance->ItemMat;

	for (int i = 0; i < ITEMACTOR_COUNT; i++)
	{
		ItemActorArr[i] = _world->SpawnActor<AItemActor>(AItemActor::StaticClass(),
			FTransform(FRotator(0, 0, 0),
				FVector(i * -120, -100000, -10000),
				FVector(1, 1, 1)));

		ItemActorArr[i]->SetMesh(Mesh);
		ItemActorArr[i]->SetMat(ItemMat);
		ItemActorArr[i]->SetActorTickEnabled(false);
		ItemActorArr[i]->SetActorHiddenInGame(true);
		ItemActorArr[i]->SetInGameLevel(InGameLevel);
		ItemActorArr[i]->SetQueueNumber(i);

		ItemSpawnEnQueue(ItemCountQueue, i);
	}

	UE_LOG(LogTemp, Warning, TEXT("ItemActor Spawn Done!"));
}

void ItemMgr::ArrayItemActor(int _x, int _y, int _cx, int _cy, GItemData _data, GItemDataNode * _node)
{
	if (!ItemSpawnQueueIsEmpty(ItemCountQueue))
	{
		int Count = ItemSpawnDeQueue(ItemCountQueue);
		int UD = ((_x % 256) / 128) * 2;
		int LR = (_y % 256) / 128;
		int UDLR = UD + LR;

		UE_LOG(LogTemp, Warning, TEXT("Actor %d %d %d"), Count, _data.ItemCode, _data.ItemAidCode);

		ItemSpawnEnQueue(ActiveItemCountQueue[_cx][_cy][UDLR], Count);
		//x와 y로 알맞는 청크와 청크 파트를 계산 후 아이템 배치
		//_x _y에 *100 해주기

		ItemActorArr[Count]->SetActorLocation(FVector(_x * 100, _y * 100, 130));
		ItemActorArr[Count]->SetTex(InGameLevel->MyGameInstance->ObjTexture[_data.ItemCode][_data.ItemAidCode][0]);
		ItemActorArr[Count]->SetData(_data);
		ItemActorArr[Count]->SetNode(_node);
		ItemActorArr[Count]->SetActorTickEnabled(true);
		ItemActorArr[Count]->SetActorHiddenInGame(false);
	}
}

void ItemMgr::HideItemChunkActor(int _x, int _y, int _lo)
{
	ItemDataListFirst(ItemData[_x][_y].Lo[_lo]);

	int ListC = ListCount(ItemData[_x][_y].Lo[_lo]);

	for (int i = 0; i < ListC; i++)
	{
		int Count = ItemSpawnDeQueue(ActiveItemCountQueue[ItemData[_x][_y].x][ItemData[_x][_y].y][_lo]);
		ItemSpawnEnQueue(ItemCountQueue, Count);

		ItemActorArr[Count]->SetActorLocation(FVector(Count * -120, -100000, -10000));
		ItemActorArr[Count]->SetActorTickEnabled(false);
		ItemActorArr[Count]->SetActorHiddenInGame(true);
	}
}

void ItemMgr::ArrayItemChunkActor(int _x, int _y, int _lo)
{
	ItemDataListFirst(ItemData[_x][_y].Lo[_lo]);

	int ListC = ListCount(ItemData[_x][_y].Lo[_lo]);

	for (int i = 0; i < ListC; i++)
	{
		int Count = ItemSpawnDeQueue(ItemCountQueue);
		ItemSpawnEnQueue(ActiveItemCountQueue[ItemData[_x][_y].x][ItemData[_x][_y].y][_lo], Count);

		GItemData data;

		ItemDataListPick(ItemData[_x][_y].Lo[_lo], &data);

		ItemActorArr[Count]->SetActorLocation(FVector(data.x * 100, data.y * 100, 130));
		ItemActorArr[Count]->SetTex(InGameLevel->MyGameInstance->ObjTexture[data.ItemCode][data.ItemAidCode][0]);
		ItemActorArr[Count]->SetData(data);
		ItemActorArr[Count]->SetNode(GetItemDataNode(ItemData[_x][_y].Lo[_lo]));
		ItemActorArr[Count]->SetActorTickEnabled(true);
		ItemActorArr[Count]->SetActorHiddenInGame(false);

		ItemDataListNext(ItemData[_x][_y].Lo[_lo], &data);
	}
}

void ItemMgr::LootingItem(int _x, int _y, int _lo, int _num)
{
	UE_LOG(LogTemp, Warning, TEXT("Looting %d %d %d %d"), _x, _y, _lo, _num);
	ItemActorArr[_num]->SetActorLocation(FVector(FVector(_num * -120, -100000, -10000)));
	ItemActorArr[_num]->SetActorTickEnabled(false);
	ItemActorArr[_num]->SetActorHiddenInGame(true);
	ItemActorArr[_num]->bOverlap = false;

	ItemSpawnDeleteQueue(ActiveItemCountQueue[_x][_y][_lo], _num);
	ItemSpawnEnQueue(ItemCountQueue, _num);
}

void ItemMgr::SetItemChunkData(GFieldItemList(*_ItemData)[3])
{
	ItemData = _ItemData;
}
