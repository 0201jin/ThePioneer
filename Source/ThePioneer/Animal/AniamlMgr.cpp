// Fill out your copyright notice in the Description page of Project Settings.

#include "AniamlMgr.h"
#include "Engine.h"

AniamlMgr::AniamlMgr(AInGameLevel * _InGameLevel)
{
	InGameLevel = _InGameLevel;
	MyGameInstance = InGameLevel->MyGameInstance;

	AnimalCountQueue = new GItemSpawnQueue;
	ItemSpawnQueueInit(AnimalCountQueue);

	for (int x = 0; x < 32; x++)
		for (int y = 0; y < 32; y++)
			for (int i = 0; i < 4; i++)
			{
				ActiveAnimalCountQueue[x][y][i] = new GItemSpawnQueue;
				ItemSpawnQueueInit(ActiveAnimalCountQueue[x][y][i]);
			}
}

AniamlMgr::~AniamlMgr()
{
}

void AniamlMgr::SpawnAnimalActor(UWorld * _world)
{
	UE_LOG(LogTemp, Warning, TEXT("AnimalActor Spawn Start!"));

	for (int i = 0; i < ANIMALACTOR_COUNT; i++)
	{
		AnimalActorArr[i] = _world->SpawnActor<AAnimalClass>(AAnimalClass::StaticClass(),
			FTransform(FRotator(0, 0, 0),
				FVector(i * -120, -100100, -100),
				FVector(1, 1, 1)));

		AnimalActorArr[i]->SetActorHiddenInGame(true);
		AnimalActorArr[i]->SetActorTickEnabled(false);

		ItemSpawnEnQueue(AnimalCountQueue, i);
	}

	UE_LOG(LogTemp, Warning, TEXT("AnimalActor Spawn Done!"));
}

void AniamlMgr::ArrayAnimalActor(int _x, int _y, int _cx, int _cy, GAnimalDataNode * _data)
{
	if (!ItemSpawnQueueIsEmpty(AnimalCountQueue))
	{
		int Count = ItemSpawnDeQueue(AnimalCountQueue);
		int UD = ((_x % 256) / 128) * 2;
		int LR = (_y % 256) / 128;
		int UDLR = UD + LR;

		UE_LOG(LogTemp, Warning, TEXT("Animal %d %d %d"), Count, _data->Data.AnimalCode);

		ItemSpawnEnQueue(ActiveAnimalCountQueue[_cx][_cy][UDLR], Count);
		//x와 y로 알맞는 청크와 청크 파트를 계산 후 아이템 배치
		//_x _y에 *100 해주기

		AnimalActorArr[Count]->SetActorLocation(FVector(_x * 100, _y * 100, 130));
		AnimalActorArr[Count]->SetChild(_data->Data.AnimalCode, _data);
		AnimalActorArr[Count]->SetData(_data);
		AnimalActorArr[Count]->SetMesh(MyGameInstance->AnimalMesh[_data->Data.AnimalCode], MyGameInstance->AnimalAnim[_data->Data.AnimalCode]);
		AnimalActorArr[Count]->SetMat(MyGameInstance->AnimalTex[_data->Data.AnimalCode], MyGameInstance->Mat[1]);
		AnimalActorArr[Count]->SetSize(_data->Data.AnimalCode);
		AnimalActorArr[Count]->StartUpdate();

		AnimalActorArr[Count]->SetActorTickEnabled(true);
		AnimalActorArr[Count]->SetActorHiddenInGame(false);
	}
}

void AniamlMgr::SetAnimalChunkData(GFieldAnimalList(*_AnimalData)[3])
{
	AnimalData = _AnimalData;
}

void AniamlMgr::HideAnimalChunkActor(int _x, int _y, int _lo)
{
	AnimalDataListFirst(AnimalData[_x][_y].Lo[_lo]);

	int ListC = ListCount(AnimalData[_x][_y].Lo[_lo]);

	for (int i = 0; i < ListC; i++)
	{
		int Count = ItemSpawnDeQueue(ActiveAnimalCountQueue[AnimalData[_x][_y].x][AnimalData[_x][_y].y][_lo]);
		ItemSpawnEnQueue(AnimalCountQueue, Count);

		AnimalActorArr[Count]->SetActorLocation(FVector(Count * -120, -100000, -10000));
		AnimalActorArr[Count]->SetActorTickEnabled(false);
		AnimalActorArr[Count]->SetActorHiddenInGame(true);
	}
}

void AniamlMgr::ArrayAnimalChunkActor(int _x, int _y, int _lo)
{
	AnimalDataListFirst(AnimalData[_x][_y].Lo[_lo]);

	int ListC = ListCount(AnimalData[_x][_y].Lo[_lo]);

	for (int i = 0; i < ListC; i++)
	{
		int Count = ItemSpawnDeQueue(AnimalCountQueue);
		ItemSpawnEnQueue(ActiveAnimalCountQueue[AnimalData[_x][_y].x][AnimalData[_x][_y].y][_lo], Count);

		GAnimalDataNode * data = AnimalDataListPick(AnimalData[_x][_y].Lo[_lo]);

		int X = data->Data.ChunkX * data->Data.TileX;
		int Y = data->Data.ChunkY * data->Data.TileY;

		UE_LOG(LogTemp, Warning, TEXT("Animal %d %d %d"), Count, data->Data.AnimalCode);

		AnimalActorArr[Count]->SetActorLocation(FVector(X * 100,Y * 100, 130));
		AnimalActorArr[Count]->SetChild(data->Data.AnimalCode, data);
		AnimalActorArr[Count]->SetData(data);
		AnimalActorArr[Count]->SetMesh(MyGameInstance->AnimalMesh[data->Data.AnimalCode], MyGameInstance->AnimalAnim[data->Data.AnimalCode]);
		AnimalActorArr[Count]->SetMat(MyGameInstance->AnimalTex[data->Data.AnimalCode], MyGameInstance->Mat[1]);
		AnimalActorArr[Count]->SetSize(data->Data.AnimalCode);
		AnimalActorArr[Count]->StartUpdate();

		AnimalActorArr[Count]->SetActorTickEnabled(true);
		AnimalActorArr[Count]->SetActorHiddenInGame(false);

		AnimalDataListNext(AnimalData[_x][_y].Lo[_lo]);
	}
}
