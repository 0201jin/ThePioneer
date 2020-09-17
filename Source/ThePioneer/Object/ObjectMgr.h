// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Object/ObjectClass.h"
#include "Object/Tile/TileObject.h"
#include "Object/Cloud/Cloud.h"

#include "Engine.h"
#include "MyGameInstance.h"

#include "LevelScript/InGameLevel.h"

#include "CoreMinimal.h"

/*타일 배치에 대한 XY크기*/
#define DISPOSETILE_X 50
#define DISPOSETILE_Y 50

/*타일 초기배치에 쓰는 매크로 카메라 조정시 필요에 따라 값을 조정*/
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

	void SpawnTileActor(UWorld * _world); //타일 스폰
	void DisposeTile(int _x, int _y); //타일 배치 딱 한번 실행
	void LR_StreamDisposeTile(int _value, int _y, int _Tile_y); //실시간 타일 배치 함수 좌우
	void FB_StreamDisposeTile(int _value, int _x, int _Tile_x); //실시간 타일 배치 함수 상하

	void SetChunkData(GChunk (*_ChunkData)[3]); //청크 데이터 설정

	void SetCloudChunkData(GCloudChunk (*_CloudChunkData)[3]); //구름 청크 데이터 설정
	void ApplyStreamCloudData(GCloudChunk(*_CloudChunkData)[3]); //실시간 구름 설정

	void SetObjectChunkData(GObjectChunk(*_ObjectChunkData)[3]); //오브젝트 청크 데이터 설정
	void ApplyStreamObjData(int _x, int _y); //실시간 오브젝트 설정

	void TempItemPlace(int _x, int _y, GItemData _data); //실시간 아이템 임시 배치 코드
	void HideTempItemPlace(); //아이템 임시 배치 숨기기

protected:
	void SetTileEdget(); //타일 테두리 설정
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
