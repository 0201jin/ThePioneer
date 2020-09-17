// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MyGameInstance.h"

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "InGameLevel.generated.h"

using namespace MapStruct;
/**
 *
 */
UCLASS()
class THEPIONEER_API AInGameLevel : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	AInGameLevel();
	~AInGameLevel();

	void LoadChunkToMapGenerator(int _x, int _y); //ObMgr 클래스에서 맵젠으로 LoadChunk를 요청
	void DisPoseTile(int _x, int _y); //ObjMgr에 DisPoseTile을 요청

	void LR_StreamDisposeTile(int _value, int _y, int _Tile_y); //실시간 타일 배치 함수 좌우
	void FB_StreamDisposeTile(int _value, int _x, int _Tile_x); //실시간 타일 배치 함수 상하

	int GetPX(); //플레이어 스폰 위치 X 반환
	int GetPY(); //플레이어 스폰 위치 Y 반환 

	int GetPTX(); //플레이어 위치 X 반환
	int GetPTY(); //플레이어 위치 Y 반환 

	void CreateCloudChunk(int _x, int _y, int _sx, int _sy); //구름 청크 생성 요청 
	void DeleteCloudChunk(int _x, int _y, int _sx, int _sy); //구름 청크 삭제 요청

	void DisposeObjChunk(int _x, int _y, GObjectData _data); //오브젝트 배치
	void DeleteObjChunk(int _x, int _y); //오브젝트 삭제
	void ChangeObjChunk(int _x, int _y, GObjectData _data); //오브젝트 변경
	bool CheckObjChunk(int _x, int _y); //해당 위치에 오브젝트를 설치 가능한지

	void WeatherToObject(GWeatherData _data, float _DeltaSecond);

	float GetPlayTime() { return PlayTime; } //플레이타임 반환

	bool GetPlayerSpawn() { return bCanSpawnPlayer; }

	GObjectData * GetObjectHead(); //오브젝트 헤드 파일

	void ObjDataBaseToItemData(GObjDataBase _ObjData, TArray<GItemData> * _ItemDataList); //오브젝트 데이터를 아이템 데이터로 변경

	void StreamItemArrayHide(); //실시간으로 아이템을 청크단위로 숨기는 코드
	void ItemSpawn(int _x, int _y, GItemDataNode * _node); //아이템을 스폰하는 코드

	void LootingItem(int _x, int _y, int _num, GItemDataNode * _node, APawn * _pawn); //아이템 루팅시 실행하는 코드

	void TempItemPlace(int _x, int _y, GItemData _data); //실시간 아이템 임시 배치 코드
	void HideTempItemPlace(); //아이템 임시 배치 숨기기

	void StreamAnimalArrayHide(); //실시간으로 동물을 청크단위로 숨기거나 배치하는 코드

	void Test();
protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;

	bool CheckInStreamChunk(int _x, int _y);
	bool CheckInItemStreamChunk(int _x, int _y);
	bool CheckInAnimalStreamChunk(int _x, int _y);

	void LoadItemChunk(int _x, int _y);
	void ItemStreamChunkToItemChunk();

	void SetAnimalChunk(GFieldAnimalList(*_AnimalData)[3]);
	GFieldItemList(*GetStreamItemChunk())[3];

public:
	UMyGameInstance * MyGameInstance;

protected:
	int MapSeed;
	int PX, PY = 0; //플레이어 스폰 위치
	int CX, CY = 0; //청크 위치
	int PTX, PTY = 0; //플레이어 타일 위치

	bool bCanSpawnPlayer = false;

	float PlayTime = 0.0f; //플레이 타임

	GFieldItemList ItemChunkData[32][32] = { 0 }; //아이템 데이터 리스트 배열
	GFieldItemList StreamItemChunkData[3][3] = { 0 }; //실시간 아이템 데이터 리스트

	GFieldAnimalList(*StreamAnimalChunkData)[3] = { 0 }; //실시간 동물 데이터 리스트
};
