// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LevelScript/InGameLevel.h"
#include "System/MapStruct.h"

#include "CoreMinimal.h"

#define MAP_SIZE 32 //���� ũ��(ûũ ũ�Ⱑ �ƴϴ�.)
#define MAP_SEED_SIZE 33 //�� ûũ�� �õ� ������. MAP_SIZE + 1

#define CHUNK_SIZE_MINUS_ONE 255
#define CHUNK_SIZE 256 //ûũ�� ũ��
#define CHUNK_SEED_SIZE 257 //ûũ�� �õ� ������

using namespace MapStruct;
/**
 *
 */
class THEPIONEER_API MapGenerator
{
public:
	MapGenerator(AInGameLevel * _InGameLevel);
	~MapGenerator();

	bool GenerateMap(int _Seed); //�� ���� �Լ�

	void LoadChunk(int _x, int _y); //ûũ�� �ε�
	GChunk(*GetStreamChunk())[3]; //��Ʈ�� ûũ ��������

	void CreateCloudChunk(int _x, int _y, int _sx, int _sy); //���� ûũ ������ ����
	void DeleteCloudChunk(int _x, int _y, int _sx, int _sy); //���� ûũ ������ ����
	void LoadCloudChunk(int _x, int _y); //���� ûũ �ε�
	GCloudChunk(*GetStreamCloudChunk())[3]; //��Ʈ�� ���� ûũ ��������

	void DisposeObjectChunk(int _x, int _y, GObjectData _data); //������Ʈ ��ġ
	void DeleteObjectChunk(int _x, int _y);						//������Ʈ ����
	void ChangeObjectData(int _x, int _y, GObjectData _data);	//������Ʈ ���� ����
	void LoadObjectChunk(int _x, int _y); //������Ʈ ûũ �ε� 
	GObjectChunk(*GetStreamObjectChunk())[3]; //��Ʈ�� ������Ʈ ûũ ��������
	GObjectData * GetObjectHead() { return ObjectHead; } //������Ʈ ��� ��ȯ
	GObjectData * GetObjectData(int _x, int _y); //������Ʈ ������ ��ȯ
	bool CheckObjChunk(int _x, int _y); //�ش� ��ġ�� ������Ʈ�� ��ġ ��������

	void WeatherToObject(GWeatherData _data, float _DeltaSecond); //������ ������Ʈ�� ������ �ִ� �Լ�

	void CreateChunk(); //ûũ Ÿ���� ����
	void MakeChunkToImage(); //ûũ �����͸� �̹����� ����

	void CreateAnimal(); //���� �����͸� ����(���� ����)
	void LoadAnimalList(int _x, int _y); //���� ������ �ε�
	GFieldAnimalList(*GetStreamFieldAnimalList())[3]; //��Ʈ�� ���� ����Ʈ ��������
	void SetAnimalChunk(GFieldAnimalList(*_AnimalData)[3]);

	void CreateWeatherEvent(int _x, int _y, int _sx, int _sy, bool _Rain); //���� �̺�Ʈ ����

	int GetPX() { return PX; }
	int GetPY() { return PY; }

protected:
	void DivisionSeed(int _Seed); //int�õ带 char�� �����ϴ� �Լ�
	void SetSeeLevel();

	void DiamondSquareAlgorithm(unsigned char _ChunkSeed[MAP_SEED_SIZE][MAP_SEED_SIZE], int size); //DiamondSquareAlgorithm�� ��� �Լ�
	void DiamondStep(unsigned char _ChunkSeed[MAP_SEED_SIZE][MAP_SEED_SIZE], int x, int y, int dis); //DiamondSetp�� �Լ�
	void SquareStep(unsigned char _ChunkSeed[MAP_SEED_SIZE][MAP_SEED_SIZE], int x, int y, int dis); //SquareStep�� �Լ�

	void CDiamondSquareAlgorithm(int size);
	void CDiamondStep(unsigned char _ChunkDataSeed[CHUNK_SEED_SIZE][CHUNK_SEED_SIZE], int x, int y, int dis, int col, int row);
	void CSquareStep(unsigned char _ChunkDataSeed[CHUNK_SEED_SIZE][CHUNK_SEED_SIZE], int x, int y, int dis);

	GChunk CreateSeedToTile(unsigned char _ChunkDataSeed[CHUNK_SEED_SIZE][CHUNK_SEED_SIZE], int _x, int _y);

	//���� �̸��� ���� �̸��� ������ ������ ��
	int SetTile(int result, int Standard, int SeeLevelHalf, int Dist, int x, int y, int _x, int _y); //Ÿ�� ���ϴ� �Լ� �� ������Ʈ�� �÷��̾� ��ġ�� �� �� �ִ� �Լ�
	void SetObject(int result, int x, int y); //�������� ������Ʈ ����
	void SetPlayerLo(int _x, int _y, int _result); //ĳ���͸� ������ ���� ��ġ��Ŵ
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

	GFieldAnimalList AnimalChunkData[32][32] = { 0 }; //���� ������ ����Ʈ �迭
	GFieldAnimalList StreamAnimalChunkData[3][3] = { 0 }; //�ǽð� ���� ������ ����Ʈ

	GWeatherList * WeatherDataList;

	GObjectData * ObjectHead;

	AInGameLevel * InGameLevel;
	UMyGameInstance * MyGameInstance;

	int SeeLevel = 0;
	int TileLevel = 0; //� Ÿ���� �� �������� ���� ����
	int Seed = 0;
	int PX, PY = 1;

	int ObjCount = 0;

	int MaxChunkX = 0;
	int MaxChunkY = 0;
	int MaxChunkCount = 0;
};
