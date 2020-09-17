// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectMgr.h"
#include "Engine.h"

ObjectMgr::ObjectMgr(AInGameLevel * _InGameLevel)
{
	InGameLevel = _InGameLevel;
	MyGameInstance = InGameLevel->MyGameInstance;
	ObjectHead = InGameLevel->GetObjectHead();
	UE_LOG(LogTemp, Warning, TEXT("ObjectMgr On"));

	for (int i = 0; i < sizeof(MyGameInstance->Texture) / sizeof(UTexture*); i++)
	{
		*(Texture + i) = *(MyGameInstance->Texture + i);
	}

	for (int i = 0; i < sizeof(MyGameInstance->Mesh) / sizeof(UStaticMesh *); i++)
	{
		*(Mesh + i) = *(MyGameInstance->Mesh + i);
	}

	for (int i = 0; i < sizeof(MyGameInstance->ObjMesh) / sizeof(UStaticMesh *); i++)
	{
		*(**ObjMesh + i) = *(**MyGameInstance->ObjMesh + i);
	}

	for (int i = 0; i < sizeof(MyGameInstance->ObjTexture) / sizeof(UTexture *); i++)
	{
		*(**ObjTexture + i) = *(**MyGameInstance->ObjTexture + i);
	}

	for (int i = 0; i < sizeof(MyGameInstance->Mat) / sizeof(UMaterial *); i++)
	{
		*(Mat + i) = *(MyGameInstance->Mat + i);
	}
}

ObjectMgr::~ObjectMgr()
{
}

void ObjectMgr::SpawnTileActor(UWorld * _world)
{
	UE_LOG(LogTemp, Warning, TEXT("TileActor Spawn Start"));

	int count = DISPOSETILE_Y * DISPOSETILE_X;

	for (int i = 0; i < count; i++)
	{
		TileArr[i] = _world->SpawnActor<ATileObject>(ATileObject::StaticClass(),
			FTransform(FRotator(0, 0, 0),
				FVector(i * -120, -10000, -10000),
				FVector(1, 1, 1)));

		TileArr[i]->SetMesh(Mesh[0]);
		TileArr[i]->SetActorHiddenInGame(true);

		CloudArr[i] = _world->SpawnActor<ACloud>(ACloud::StaticClass(),
			FTransform(FRotator(0, 0, 0),
				FVector(i * -120, -10100, -10000),
				FVector(1, 1, 1)));
		CloudArr[i]->SetMesh(Mesh[1]);
		CloudArr[i]->SetActorHiddenInGame(true);

		ActorObjArr[i] = _world->SpawnActor<AObjectClass>(AObjectClass::StaticClass(),
			FTransform(FRotator(0, 0, 0),
				FVector(i * -120, -10200, -10000),
				FVector(1, 1, 1)));

		ActorObjArr[i]->SetFadeMat(Mat[0]);
		ActorObjArr[i]->SetMesh(Mesh[1]);
		ActorObjArr[i]->SetActorHiddenInGame(true);
	}

	TempItem = _world->SpawnActor<AObjectClass>(AObjectClass::StaticClass(),
		FTransform(FRotator(0, 0, 0),
			FVector(-20, -10200, -10000),
			FVector(1, 1, 1)));

	TempItem->SetFadeMat(Mat[0]);
	TempItem->SetMesh(Mesh[1]);
	TempItem->SetActorHiddenInGame(true);

	UE_LOG(LogTemp, Warning, TEXT("TileActor Spawn Done!"));
}

void ObjectMgr::DisposeTile(int _x, int _y)
{
	int XAdjust = 0;
	int YAdjust = 0;

	if (_x - ADJUST_X_M < 0)
		XAdjust = ADJUST_X_M - (_x);

	else if (_x + ADJUST_X_P > 8192)
		XAdjust = (8192 - ADJUST_X_P) - _x;

	if (_y - ADJUST_Y_M < 0)
		YAdjust = ADJUST_Y_M - _y;

	else if (_y + ADJUST_Y_P > 8192)
		YAdjust = (8192 - ADJUST_Y_P) - _y;

	int BlockCount = 0;
	int ObjectCount = 0;

	for (int x = _x - ADJUST_X_M + XAdjust; x < _x + ADJUST_X_P + XAdjust; x++)
	{
		for (int y = _y - ADJUST_Y_M + YAdjust; y < _y + ADJUST_Y_P + YAdjust; y++)
		{
			TileArr[BlockCount]->SetActorLocation(
				FVector(
				(x * 100),
					(y * 100),
					0));

			int ActorX = TileArr[BlockCount]->GetActorLocation().X * 0.01;
			int ActorY = TileArr[BlockCount]->GetActorLocation().Y * 0.01;

			int ChunkY = y / 256;
			int ChunkX = x / 256;

			int ChunkYSub = ChunkY - ChunkData[1][1].y;
			int ChunkXSub = ChunkX - ChunkData[1][1].x;

			int TileX = x % 256;
			int TileY = y % 256;

			TileArr[BlockCount]->SetActorHiddenInGame(false);
			TileArr[BlockCount]->SetMat(Texture[ChunkData[1 + ChunkXSub][1 + ChunkYSub].Tile[TileX][TileY]]);

			if (CloudChunkData[1 + ChunkXSub][1 + ChunkYSub].Tile[TileX][TileY] > 0)
			{
				CloudArr[CloudCount]->SetActorLocation(FVector(
					(x * 100),
					(y * 100),
					1500));

				CloudArr[CloudCount]->SetActorHiddenInGame(false);
			}

			SetObjectChunk(ObjectCount, ChunkYSub, ChunkXSub, TileX, TileY, x, y); //������Ʈ ����

			ObjectCount++;
			CloudCount++;
			BlockCount++;
		}
	}

	SetTileEdget();
}

void ObjectMgr::LR_StreamDisposeTile(int _value, int _y, int _Tile_y)
{
	PY = _y;
	//value�� �¿츦 ������.
	if (_value > 0 && _y > DISPOSETILE_Y * 0.5 && _y < 8192 - (DISPOSETILE_Y * 0.5)) //��
	{
		//Ÿ���� ��� ��ĭ �δ�.
		//���� Ÿ���� ���������� �̵�

		//���⿡ 0�� �������� �ؼ� �ű�� �ڵ� �ۼ�

		for (int i = 0; i < DISPOSETILE_X; i++)
		{
			int x = (TileEdgeStandardArr[0] + (DISPOSETILE_Y * i)) % (DISPOSETILE_Y * DISPOSETILE_X);

			TileArr[x]->AddActorWorldOffset(FVector(0, DISPOSETILE_Y * 100, 0));

			//Ÿ���� ������ �����ϴ� �ڵ� �ۼ�
			int ChunkY = (TileArr[x]->GetActorLocation().Y * 0.01) / 256;
			int ChunkX = (TileArr[x]->GetActorLocation().X * 0.01) / 256;

			int ChunkYSub = ChunkY - ChunkData[1][1].y;
			int ChunkXSub = ChunkX - ChunkData[1][1].x;

			int ActorX = TileArr[x]->GetActorLocation().X * 0.01;
			int ActorY = TileArr[x]->GetActorLocation().Y * 0.01;

			int TileX = ActorX % 256;
			int TileY = ActorY % 256;

			TileArr[x]->SetMat(Texture[ChunkData[1 + ChunkXSub][1 + ChunkYSub].Tile[TileX][TileY]]);

			if (CloudChunkData[1 + ChunkXSub][1 + ChunkYSub].Tile[TileX][TileY] > 0)
			{
				CloudArr[x]->SetActorLocation(FVector(
					(ActorX * 100),
					(ActorY * 100),
					1500));

				CloudArr[x]->SetActorHiddenInGame(false);
			}
			else
			{
				CloudArr[x]->SetActorHiddenInGame(true);
			}

			SetObjectChunk(x, ChunkYSub, ChunkXSub, TileX, TileY, ActorX, ActorY); //������Ʈ ����
		}

		TileEdgeStandardArr[0] = ((TileEdgeStandardArr[0] + 1) % DISPOSETILE_Y) + (((TileEdgeStandardArr[0] / DISPOSETILE_Y)) *  DISPOSETILE_Y);
		SetTileEdget();
	}

	else  if (_value < 0 && _y >(DISPOSETILE_Y * 0.5) - 1 && _y < 8191 - (DISPOSETILE_Y * 0.5)) //��
	{
		//Ÿ���� �·� ��ĭ �δ�.
		//������ Ÿ���� �������� �̵�

		TileEdgeStandardArr[0] = TileEdgeStandardArr[1];

		SetTileEdget();

		//���⿡ 0�� �������� �ؼ� �ű�� �ڵ� �ۼ�

		for (int i = 0; i < DISPOSETILE_X; i++)
		{
			int x = (TileEdgeStandardArr[0] + (DISPOSETILE_Y * i)) % (DISPOSETILE_Y * DISPOSETILE_X);

			TileArr[x]->AddActorWorldOffset(FVector(0, DISPOSETILE_Y * -100, 0));

			//Ÿ���� ������ �����ϴ� �ڵ� �ۼ�
			int ChunkY = (TileArr[x]->GetActorLocation().Y * 0.01) / 256;
			int ChunkX = (TileArr[x]->GetActorLocation().X * 0.01) / 256;

			int ChunkYSub = ChunkY - ChunkData[1][1].y;
			int ChunkXSub = ChunkX - ChunkData[1][1].x;

			int ActorX = TileArr[x]->GetActorLocation().X * 0.01;
			int ActorY = TileArr[x]->GetActorLocation().Y * 0.01;

			int TileX = ActorX % 256;
			int TileY = ActorY % 256;

			TileArr[x]->SetMat(Texture[ChunkData[1 + ChunkXSub][1 + ChunkYSub].Tile[TileX][TileY]]);

			if (CloudChunkData[1 + ChunkXSub][1 + ChunkYSub].Tile[TileX][TileY] > 0)
			{
				CloudArr[x]->SetActorLocation(FVector(
					(ActorX * 100),
					(ActorY * 100),
					1500));

				CloudArr[x]->SetActorHiddenInGame(false);
			}
			else
			{
				CloudArr[x]->SetActorHiddenInGame(true);
			}

			SetObjectChunk(x, ChunkYSub, ChunkXSub, TileX, TileY, ActorX, ActorY); //������Ʈ ����
		}
	}
}

void ObjectMgr::FB_StreamDisposeTile(int _value, int _x, int _Tile_x)
{
	PX = _x;
	//value�� ���ϸ� ������.
	if (_value > 0 && _x > DISPOSETILE_X * 0.5 && _x < 8192 - (DISPOSETILE_X * 0.5)) //��
	{
		//Ÿ���� ������ ��ĭ �δ�.
		//�Ʒ��� Ÿ���� ���ʷ� �̵�

		//���⿡ 0�� �������� �ؼ� �ű�� �ڵ� �ۼ�

		for (int i = 0; i < DISPOSETILE_Y; i++)
		{
			int y = ((TileEdgeStandardArr[0] + i) % DISPOSETILE_Y) + ((TileEdgeStandardArr[0] / DISPOSETILE_Y) *  DISPOSETILE_Y);

			TileArr[y]->AddActorWorldOffset(FVector(DISPOSETILE_X * 100, 0, 0));

			//Ÿ���� ������ �����ϴ� �ڵ� �ۼ�
			int ChunkY = (TileArr[y]->GetActorLocation().Y * 0.01) / 256;
			int ChunkX = (TileArr[y]->GetActorLocation().X * 0.01) / 256;

			int ChunkYSub = ChunkY - ChunkData[1][1].y;
			int ChunkXSub = ChunkX - ChunkData[1][1].x;

			int ActorX = TileArr[y]->GetActorLocation().X * 0.01;
			int ActorY = TileArr[y]->GetActorLocation().Y * 0.01;

			int TileX = ActorX % 256;
			int TileY = ActorY % 256;

			TileArr[y]->SetMat(Texture[ChunkData[1 + ChunkXSub][1 + ChunkYSub].Tile[TileX][TileY]]);

			if (CloudChunkData[1 + ChunkXSub][1 + ChunkYSub].Tile[TileX][TileY] > 0)
			{
				CloudArr[y]->SetActorLocation(FVector(
					(ActorX * 100),
					(ActorY * 100),
					1500));

				CloudArr[y]->SetActorHiddenInGame(false);
			}
			else
			{
				CloudArr[y]->SetActorHiddenInGame(true);
			}

			SetObjectChunk(y, ChunkYSub, ChunkXSub, TileX, TileY, ActorX, ActorY); //������Ʈ ����
		}

		TileEdgeStandardArr[0] = (TileEdgeStandardArr[0] + DISPOSETILE_Y) % (DISPOSETILE_Y * DISPOSETILE_X);

		SetTileEdget();
	}

	if (_value < 0 && _x >(DISPOSETILE_X * 0.5) - 1 && _x < 8191 - (DISPOSETILE_X * 0.5)) //��
	{
		//Ÿ���� �Ϸ� ��ĭ �δ�.
		//���� Ÿ���� �Ʒ������� �̵�

		TileEdgeStandardArr[0] = TileEdgeStandardArr[2];

		SetTileEdget();

		//���⿡ 0�� �������� �ؼ� �ű�� �ڵ� �ۼ�

		for (int i = 0; i < DISPOSETILE_Y; i++)
		{
			int y = ((TileEdgeStandardArr[0] + i) % DISPOSETILE_Y) + ((TileEdgeStandardArr[0] / DISPOSETILE_Y) *  DISPOSETILE_Y);

			TileArr[y]->AddActorWorldOffset(FVector(DISPOSETILE_X * -100, 0, 0));

			//Ÿ���� ������ �����ϴ� �ڵ� �ۼ�
			int ChunkY = (TileArr[y]->GetActorLocation().Y * 0.01) / 256;
			int ChunkX = (TileArr[y]->GetActorLocation().X * 0.01) / 256;

			int ChunkYSub = ChunkY - ChunkData[1][1].y;
			int ChunkXSub = ChunkX - ChunkData[1][1].x;

			int ActorX = TileArr[y]->GetActorLocation().X * 0.01;
			int ActorY = TileArr[y]->GetActorLocation().Y * 0.01;

			int TileX = ActorX % 256;
			int TileY = ActorY % 256;

			TileArr[y]->SetMat(Texture[ChunkData[1 + ChunkXSub][1 + ChunkYSub].Tile[TileX][TileY]]);

			if (CloudChunkData[1 + ChunkXSub][1 + ChunkYSub].Tile[TileX][TileY] > 0)
			{
				CloudArr[y]->SetActorLocation(FVector(
					(ActorX * 100),
					(ActorY * 100),
					1500));

				CloudArr[y]->SetActorHiddenInGame(false);
			}
			else
			{
				CloudArr[y]->SetActorHiddenInGame(true);
			}

			SetObjectChunk(y, ChunkYSub, ChunkXSub, TileX, TileY, ActorX, ActorY); //������Ʈ ����
		}
	}
}

void ObjectMgr::SetChunkData(GChunk(*_ChunkData)[3])
{
	ChunkData = _ChunkData;
}

void ObjectMgr::SetCloudChunkData(GCloudChunk(*_CloudChunkData)[3])
{
	CloudChunkData = _CloudChunkData;
}

void ObjectMgr::SetObjectChunkData(GObjectChunk(*_ObjectChunkData)[3])
{
	ObjectChunkData = _ObjectChunkData;
}

void ObjectMgr::ApplyStreamObjData(int _x, int _y)
{
	int x = (_x - (TileArr[TileEdgeStandardArr[0]]->GetActorLocation().X * 0.01));
	int y = (_y - (TileArr[TileEdgeStandardArr[0]]->GetActorLocation().Y * 0.01));

	int DisX = (DISPOSETILE_Y * (x + (TileEdgeStandardArr[0] / DISPOSETILE_Y))) % (DISPOSETILE_Y * DISPOSETILE_X);
	int DisY = (TileEdgeStandardArr[0] + y) % (DISPOSETILE_Y);

	int count = DisX + DisY;

	int ChunkX = _x / 256;
	int ChunkY = _y / 256;

	int ChunkXSub = ChunkX - ChunkData[1][1].x;
	int ChunkYSub = ChunkY - ChunkData[1][1].y;

	int TileX = _x % 256;
	int TileY = _y % 256;

	if ((TileArr[count]->GetActorLocation().X * 0.01) == _x &&
		(TileArr[count]->GetActorLocation().Y * 0.01) == _y)
	{
		SetObjectChunk(count, ChunkYSub, ChunkXSub, TileX, TileY, _x, _y); //������Ʈ ����
	}
}

void ObjectMgr::TempItemPlace(int _x, int _y, GItemData _data)
{
	if (InGameLevel->CheckObjChunk(_x * 0.01, _y * 0.01) && 
		!(InGameLevel->GetPTX() == _x * 0.01 && InGameLevel->GetPTY() == _y * 0.01))
	{
		TempItem->SetActorLocation(FVector(_x, _y, 100));
		GObjDataBase ODB = MyGameInstance->ObjDataBase[_data.ItemCode][_data.ItemAidCode];

		TempItem->SetMesh(ObjMesh[ODB.DisposeItemCode][ODB.DisposeItemAidCode][0]);
		TempItem->SetMat(ObjTexture[ODB.DisposeItemCode][ODB.DisposeItemAidCode][0], Mat[ODB.Obj_Mat_Num]);
		TempItem->SetSize(ODB.DisposeItemCode, ODB.DisposeItemAidCode);

		TempItem->SetActorEnableCollision(false);

		TempItem->FadeIn();
		TempItem->SetActorHiddenInGame(false);
	}

	else
	{
		TempItem->SetActorHiddenInGame(true);
	}
}

void ObjectMgr::HideTempItemPlace()
{
	TempItem->SetActorHiddenInGame(true);
}

void ObjectMgr::ApplyStreamCloudData(GCloudChunk(*_CloudChunkData)[3])
{
	CloudChunkData = _CloudChunkData;

	UE_LOG(LogTemp, Warning, TEXT("ApplyStreamCloudData"));

	for (int x = 0; x < DISPOSETILE_X; x++)
	{
		for (int y = 0; y < DISPOSETILE_Y; y++)
		{
			int DisX = (DISPOSETILE_Y * (x + (TileEdgeStandardArr[0] / DISPOSETILE_Y))) % (DISPOSETILE_Y * DISPOSETILE_X);
			int DisY = (TileEdgeStandardArr[0] + y) % (DISPOSETILE_Y);

			int xx = DisX + DisY;

			int ActorX = TileArr[xx]->GetActorLocation().X * 0.01;
			int ActorY = TileArr[xx]->GetActorLocation().Y * 0.01;

			int ChunkY = ActorY / 256;
			int ChunkX = ActorX / 256;

			int ChunkYSub = ChunkY - ChunkData[1][1].y;
			int ChunkXSub = ChunkX - ChunkData[1][1].x;

			int TileX = ActorX % 256;
			int TileY = ActorY % 256;

			if (CloudChunkData[1 + ChunkXSub][1 + ChunkYSub].Tile[TileX][TileY] > 0)
			{
				CloudArr[xx]->SetActorLocation(FVector(
					(ActorX * 100),
					(ActorY * 100),
					1500));

				CloudArr[xx]->SetActorHiddenInGame(false);
			}
			else
			{
				CloudArr[xx]->SetActorHiddenInGame(true);
			}
		}
	}
}

void ObjectMgr::SetTileEdget()
{
	//���� �ϴ�
	TileEdgeStandardArr[1] = ((TileEdgeStandardArr[0] + DISPOSETILE_Y - 1) % DISPOSETILE_Y) + (((TileEdgeStandardArr[0] / DISPOSETILE_Y)) *  DISPOSETILE_Y);

	//���� ���
	TileEdgeStandardArr[2] = (TileEdgeStandardArr[0] + (DISPOSETILE_Y * (DISPOSETILE_X - 1))) % (DISPOSETILE_Y * DISPOSETILE_X);

	//���� ���
	TileEdgeStandardArr[3] = (TileEdgeStandardArr[1] + (DISPOSETILE_Y * (DISPOSETILE_X - 1))) % (DISPOSETILE_Y * DISPOSETILE_X);
}

void ObjectMgr::SetObjectChunk(int _count, int _ChunkYSub, int _ChunkXSub, int _TileX, int _TileY, int _ActorX, int _ActorY)
{
	//UE_LOG(LogTemp, Warning, TEXT("%d"), ObjectChunkData[1 + _ChunkXSub][1 + _ChunkYSub].Tile[_TileX][_TileY]);
	//���⼭�� list Head + �迭���� ���ؼ� ������ ��´�.
	if (ObjectChunkData[1 + _ChunkXSub][1 + _ChunkYSub].Tile[_TileX][_TileY] != 0)
	{
		ActorObjArr[_count]->SetActorLocation(FVector(
			(_ActorX * 100),
			(_ActorY * 100),
			100));

		GObjectData * ObjectData = (ObjectChunkData[1 + _ChunkXSub][1 + _ChunkYSub].Tile[_TileX][_TileY]);
		GObjectData testObjectData = *(ObjectChunkData[1 + _ChunkXSub][1 + _ChunkYSub].Tile[_TileX][_TileY]);

		ActorObjArr[_count]->SetChild((*ObjectData).Code, (*ObjectData).AidCode, ObjectData);

		ActorObjArr[_count]->SetData(ObjectData);

		ActorObjArr[_count]->SetMesh(ObjMesh[(*ObjectData).Code][(*ObjectData).AidCode][(*ObjectData).Grow]);

		ActorObjArr[_count]->SetMat(ObjTexture[(*ObjectData).Code][(*ObjectData).AidCode][(*ObjectData).Grow], Mat[MyGameInstance->ObjDataBase[(*ObjectData).Code][(*ObjectData).AidCode].Obj_Mat_Num]);

		ActorObjArr[_count]->SetSize((*ObjectData).Code, (*ObjectData).AidCode);

		ActorObjArr[_count]->StartUpdate();

		ActorObjArr[_count]->SetActorHiddenInGame(false);

		ActorObjArr[_count]->SetActorTickEnabled(true);
	}
	else
	{
		ActorObjArr[_count]->SetActorLocation(FVector(_count * -120, -10200, -10000));
		ActorObjArr[_count]->SetMesh(ObjMesh[0][0][0]);
		ActorObjArr[_count]->SetActorHiddenInGame(true);
		ActorObjArr[_count]->SetActorTickEnabled(false);
	}
}
