// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Object/ObjectClass.h"
#include "Object/Tile/TileObject.h"
#include "Object/Cloud/Cloud.h"

#include "Engine.h"
#include "MyGameInstance.h"

#include "LevelScript/InGameLevel.h"

#include "CoreMinimal.h"

/*Ÿ�� ��ġ�� ���� XYũ��*/
#define DISPOSETILE_X 50
#define DISPOSETILE_Y 50

/*Ÿ�� �ʱ��ġ�� ���� ��ũ�� ī�޶� ������ �ʿ信 ���� ���� ����*/
#define ADJUST_X_M 25
#define ADJUST_X_P 25
#define ADJUST_Y_M 25
#define ADJUST_Y_P 25

using namespace MapStruct;
/**
 * 
 */
class THEPIONEER_API ObjectMgr
{
public:
	ObjectMgr(AInGameLevel * _InGameLevel);
	~ObjectMgr();

	void SpawnTileActor(UWorld * _world); //Ÿ�� ����
	void DisposeTile(int _x, int _y); //Ÿ�� ��ġ �� �ѹ� ����
	void LR_StreamDisposeTile(int _value, int _y, int _Tile_y); //�ǽð� Ÿ�� ��ġ �Լ� �¿�
	void FB_StreamDisposeTile(int _value, int _x, int _Tile_x); //�ǽð� Ÿ�� ��ġ �Լ� ����

	void SetChunkData(GChunk (*_ChunkData)[3]); //ûũ ������ ����

	void SetCloudChunkData(GCloudChunk (*_CloudChunkData)[3]); //���� ûũ ������ ����
	void ApplyStreamCloudData(GCloudChunk(*_CloudChunkData)[3]); //�ǽð� ���� ����

	void SetObjectChunkData(GObjectChunk(*_ObjectChunkData)[3]); //������Ʈ ûũ ������ ����
	void ApplyStreamObjData(int _x, int _y); //�ǽð� ������Ʈ ����

	void TempItemPlace(int _x, int _y, GItemData _data); //�ǽð� ������ �ӽ� ��ġ �ڵ�
	void HideTempItemPlace(); //������ �ӽ� ��ġ �����

protected:
	void SetTileEdget(); //Ÿ�� �׵θ� ����
	void SetObjectChunk(int _count, int _ChunkYSub, int _ChunkXSub, int _TileX, int _TileY, int _ActorX, int _ActorY);

protected:
	ATileObject * TileArr[DISPOSETILE_Y * DISPOSETILE_X];
	ACloud * CloudArr[DISPOSETILE_Y * DISPOSETILE_X];
	AObjectClass * ActorObjArr[DISPOSETILE_Y * DISPOSETILE_X];

	int TileEdgeStandardArr[4] = { 0 };

	int CloudCount = 0;

	int PX, PY = 0;

	GChunk (*ChunkData)[3];
	GCloudChunk (*CloudChunkData)[3];
	GObjectChunk(*ObjectChunkData)[3];

	AObjectClass * TempItem;

	GObjectData * ObjectHead;

	AInGameLevel * InGameLevel;
	UMyGameInstance * MyGameInstance;

	UTexture * Texture[TEXTURE_SIZE];
	UStaticMesh * Mesh[MESH_SIZE];
	UStaticMesh * ObjMesh[OBJ_MESH_FIRST][OBJ_MESH_SECOND][OBJ_MESH_THIRD] = { (nullptr), (nullptr) };
	UTexture * ObjTexture[OBJ_TEXTURE_FIRST][OBJ_TEXTURE_SECOND][OBJ_TEXTURE_THIRD];
	UMaterial * Mat[MAT_SIZE];
};
