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

	void LoadChunkToMapGenerator(int _x, int _y); //ObMgr Ŭ�������� �������� LoadChunk�� ��û
	void DisPoseTile(int _x, int _y); //ObjMgr�� DisPoseTile�� ��û

	void LR_StreamDisposeTile(int _value, int _y, int _Tile_y); //�ǽð� Ÿ�� ��ġ �Լ� �¿�
	void FB_StreamDisposeTile(int _value, int _x, int _Tile_x); //�ǽð� Ÿ�� ��ġ �Լ� ����

	int GetPX(); //�÷��̾� ���� ��ġ X ��ȯ
	int GetPY(); //�÷��̾� ���� ��ġ Y ��ȯ 

	int GetPTX(); //�÷��̾� ��ġ X ��ȯ
	int GetPTY(); //�÷��̾� ��ġ Y ��ȯ 

	void CreateCloudChunk(int _x, int _y, int _sx, int _sy); //���� ûũ ���� ��û 
	void DeleteCloudChunk(int _x, int _y, int _sx, int _sy); //���� ûũ ���� ��û

	void DisposeObjChunk(int _x, int _y, GObjectData _data); //������Ʈ ��ġ
	void DeleteObjChunk(int _x, int _y); //������Ʈ ����
	void ChangeObjChunk(int _x, int _y, GObjectData _data); //������Ʈ ����
	bool CheckObjChunk(int _x, int _y); //�ش� ��ġ�� ������Ʈ�� ��ġ ��������

	void WeatherToObject(GWeatherData _data, float _DeltaSecond);

	float GetPlayTime() { return PlayTime; } //�÷���Ÿ�� ��ȯ

	bool GetPlayerSpawn() { return bCanSpawnPlayer; }

	GObjectData * GetObjectHead(); //������Ʈ ��� ����

	void ObjDataBaseToItemData(GObjDataBase _ObjData, TArray<GItemData> * _ItemDataList); //������Ʈ �����͸� ������ �����ͷ� ����

	void StreamItemArrayHide(); //�ǽð����� �������� ûũ������ ����� �ڵ�
	void ItemSpawn(int _x, int _y, GItemDataNode * _node); //�������� �����ϴ� �ڵ�

	void LootingItem(int _x, int _y, int _num, GItemDataNode * _node, APawn * _pawn); //������ ���ý� �����ϴ� �ڵ�

	void TempItemPlace(int _x, int _y, GItemData _data); //�ǽð� ������ �ӽ� ��ġ �ڵ�
	void HideTempItemPlace(); //������ �ӽ� ��ġ �����

	void StreamAnimalArrayHide(); //�ǽð����� ������ ûũ������ ����ų� ��ġ�ϴ� �ڵ�

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
	int PX, PY = 0; //�÷��̾� ���� ��ġ
	int CX, CY = 0; //ûũ ��ġ
	int PTX, PTY = 0; //�÷��̾� Ÿ�� ��ġ

	bool bCanSpawnPlayer = false;

	float PlayTime = 0.0f; //�÷��� Ÿ��

	GFieldItemList ItemChunkData[32][32] = { 0 }; //������ ������ ����Ʈ �迭
	GFieldItemList StreamItemChunkData[3][3] = { 0 }; //�ǽð� ������ ������ ����Ʈ

	GFieldAnimalList(*StreamAnimalChunkData)[3] = { 0 }; //�ǽð� ���� ������ ����Ʈ
};
