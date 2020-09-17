// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LevelScript/InGameLevel.h"
#include "System/MapStruct.h"

#include "CoreMinimal.h"

#define MAP_SIZE 32 //맵의 크기(청크 크기가 아니다.)
#define MAP_SEED_SIZE 33 //맵 청크의 시드 사이즈. MAP_SIZE + 1

#define CHUNK_SIZE_MINUS_ONE 255
#define CHUNK_SIZE 256 //청크의 크기
#define CHUNK_SEED_SIZE 257 //청크의 시드 사이즈

using namespace MapStruct;
/**
 *
 */
class THEPIONEER_API MapGenerator
{
public:
	MapGenerator(AInGameLevel * _InGameLevel);
	~MapGenerator();

	bool GenerateMap(int _Seed); //맵 생성 함수

	void LoadChunk(int _x, int _y); //청크를 로드
	GChunk(*GetStreamChunk())[3]; //스트림 청크 가져오기

	void CreateCloudChunk(int _x, int _y, int _sx, int _sy); //구름 청크 데이터 생성
	void DeleteCloudChunk(int _x, int _y, int _sx, int _sy); //구름 청크 데이터 삭제
	void LoadCloudChunk(int _x, int _y); //구름 청크 로드
	GCloudChunk(*GetStreamCloudChunk())[3]; //스트림 구름 청크 가져오기

	void DisposeObjectChunk(int _x, int _y, GObjectData _data); //오브젝트 배치
	void DeleteObjectChunk(int _x, int _y);						//오브젝트 삭제
	void ChangeObjectData(int _x, int _y, GObjectData _data);	//오브젝트 정보 수정
	void LoadObjectChunk(int _x, int _y); //오브젝트 청크 로드 
	GObjectChunk(*GetStreamObjectChunk())[3]; //스트림 오브젝트 청크 가져오기
	GObjectData * GetObjectHead() { return ObjectHead; } //오브젝트 헤드 반환
	GObjectData * GetObjectData(int _x, int _y); //오브젝트 데이터 반환
	bool CheckObjChunk(int _x, int _y); //해당 위치에 오브젝트를 설치 가능한지

	void WeatherToObject(GWeatherData _data, float _DeltaSecond); //날씨가 오브젝트에 영향을 주는 함수

	void CreateChunk(); //청크 타일을 생성
	void MakeChunkToImage(); //청크 데이터를 이미지로 생성

	void CreateAnimal(); //동물 데이터를 생성(최초 생성)
	void LoadAnimalList(int _x, int _y); //동물 데이터 로드
	GFieldAnimalList(*GetStreamFieldAnimalList())[3]; //스트림 동물 리스트 가져오기
	void SetAnimalChunk(GFieldAnimalList(*_AnimalData)[3]);

	void CreateWeatherEvent(int _x, int _y, int _sx, int _sy, bool _Rain); //날씨 이벤트 생성

	int GetPX() { return PX; }
	int GetPY() { return PY; }

protected:
	void DivisionSeed(int _Seed); //int시드를 char로 분할하는 함수
	void SetSeeLevel();

	void DiamondSquareAlgorithm(unsigned char _ChunkSeed[MAP_SEED_SIZE][MAP_SEED_SIZE], int size); //DiamondSquareAlgorithm의 재귀 함수
	void DiamondStep(unsigned char _ChunkSeed[MAP_SEED_SIZE][MAP_SEED_SIZE], int x, int y, int dis); //DiamondSetp의 함수
	void SquareStep(unsigned char _ChunkSeed[MAP_SEED_SIZE][MAP_SEED_SIZE], int x, int y, int dis); //SquareStep의 함수

	void CDiamondSquareAlgorithm(int size);
	void CDiamondStep(unsigned char _ChunkDataSeed[CHUNK_SEED_SIZE][CHUNK_SEED_SIZE], int x, int y, int dis, int col, int row);
	void CSquareStep(unsigned char _ChunkDataSeed[CHUNK_SEED_SIZE][CHUNK_SEED_SIZE], int x, int y, int dis);

	GChunk CreateSeedToTile(unsigned char _ChunkDataSeed[CHUNK_SEED_SIZE][CHUNK_SEED_SIZE], int _x, int _y);

	//변수 이름과 같은 이름의 변수를 넣으면 됨
	int SetTile(int result, int Standard, int SeeLevelHalf, int Dist, int x, int y, int _x, int _y); //타일 정하는 함수 및 오브젝트와 플레이어 배치를 할 수 있는 함수
	void SetObject(int result, int x, int y); //랜덤으로 오브젝트 생성
	void SetPlayerLo(int _x, int _y, int _result); //캐릭터를 정당한 곳에 위치시킴
	void SetFinalPlayerLo();

protected:
	unsigned char ChunkSeed[MAP_SEED_SIZE][MAP_SEED_SIZE] = { 2 };

	int ChunkObjCount[32][32] = { 0 };

	GChunk ChunkData[32][32] = { 0 };
	GChunk StreamChunkData[3][3] = { 0 };
	GSeedChunk SeedChunkData[32][32] = { 0 };

	GCloudChunk CloudChunkData[32][32] = { 0 };
	GCloudChunk StreamCloud[3][3] = { 0 };

	GObjectChunk ObjectChunkData[32][32];
	GObjectChunk StreamObject[3][3] = { 0 };

	GFieldAnimalList AnimalChunkData[32][32] = { 0 }; //동물 데이터 리스트 배열
	GFieldAnimalList StreamAnimalChunkData[3][3] = { 0 }; //실시간 동물 데이터 리스트

	GWeatherList * WeatherDataList;

	GObjectData * ObjectHead;

	AInGameLevel * InGameLevel;
	UMyGameInstance * MyGameInstance;

	int SeeLevel = 0;
	int TileLevel = 0; //어떤 타일을 깔 것인지에 대한 기준
	int Seed = 0;
	int PX, PY = 1;

	int ObjCount = 0;

	int MaxChunkX = 0;
	int MaxChunkY = 0;
	int MaxChunkCount = 0;
};
