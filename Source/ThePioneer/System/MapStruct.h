// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "DataTable/ItemDataStruct.h"

#include "CoreMinimal.h"

/*���ο� ������Ʈ ������ ����*/
#define WATER_OBJ_COUNT 0
#define DESERT_OBJ_COUNT 1
#define GRASS_OBJ_COUNT 3
#define SNOW_OBJ_COUNT 2
#define ICE_OBJ_COUNT 0

#define INVEN_COL 5
#define INVEN_ROW 6
#define INVEN_SIZE INVEN_COL * INVEN_ROW

/**
 *
 */
namespace MapStruct
{
	typedef struct _ObjDataBase
	{
		unsigned char ItemCode = 0;
		unsigned char ItemAidCode = 0;
		unsigned char DisposeItemCode = 0;
		unsigned char DisposeItemAidCode = 0;

		unsigned char Obj_Max_Grow = 0; //�ִ� ����
		unsigned char Obj_Mat_Num = 0; //��Ʈ���� �ѹ�
		bool Can_Dispose = false;
		bool Can_Install[5] = { false, false, false, false, false }; //�� �縷 �ʿ� �� ����

		TArray<FItemCode> DestItemCode;
		TArray<FItemCode> PriceDestItemCode;
		unsigned char Used = 0;
		unsigned char Attack = 0;

		bool Can_Wear = false;
		unsigned char Parts = 0;
		unsigned char Sheild = 0;
		unsigned char Distance = 0;
		unsigned char Health = 0;

		unsigned char Amount = 0;
		unsigned char Max_Amount = 0;
	}GObjDataBase;

	//ItemCode�� ������ �ִ´�.
	//������Ʈ�� �ʱ⿡ ��ġ�Ҷ� ���
	static const int Water_Obj[64] =
	{

	};

	static const int Desert_Obj[64] =
	{
		4
	};

	static const int Grass_Obj[64] =
	{
		1, 2, 4
	};

	static const int Snow_Obj[64] =
	{
		2, 4
	};

	static const int Ice_Obj[64] =
	{

	};

	struct _ItemDataNode;
	struct _WeatherList;
	struct _ItemDataList;

	typedef struct _ItemData
	{
		unsigned char ItemCode = 0;			//������ �ڵ�
		unsigned char ItemAidCode = 0;		//���� ������ �ڵ�
		unsigned char Amount = 0;			//������ ���� Ȥ�� ������

		unsigned char Attack = 0;			//�������� ���ݷ�
		unsigned char Sheild = 0;			//�������� ����
		unsigned char Distance = 0;			//�������� ��Ÿ�
		unsigned char Health = 0;			//�������� ü��

		int x = 0;							//�������� ��ġ(��ٴ�)
		int y = 0;							//�������� ��ġ(��ٴ�)

		_ItemDataNode * NodeAdr;			//����� �ּڰ�
	}GItemData;

	typedef struct _Chunk //ûũ ����ü
	{
		unsigned char Tile[256][256] = { 0 };
		int x, y;
	}GChunk;

	typedef struct _ObjectData
	{
		unsigned char Code = 0;
		unsigned char Grow = 0;
		unsigned char Exp = 0;
		unsigned char FExp = 0;

		float Time = 0.0f;

		void * ObjAdr = nullptr;

		unsigned char AidCode = 0;
		//3Byte
	}GObjectData;

	typedef struct _ObjectChunk
	{
		GObjectData * Tile[256][256] = { 0 };
	}GObjectChunk;

	typedef struct _SeedChunk //ûũ �õ� ������
	{
		unsigned char Seed[257][257] = { 0 };
	}GSeedChunk;

	typedef struct _CloudChunk //���� ûũ ������
	{
		unsigned char Tile[256][256] = { 0 };
		int x, y;
	}GCloudChunk;

	typedef struct _WeatherData //���� ������
	{
		unsigned char Rain;
		int x, y; //�߽�
		int sx, sy; //����
		float time; //�̺�Ʈ ���� �ð� (for�� ���� Ƚ��)
	}GWeatherData;

	typedef struct _WeatherNode //���� ���
	{
		GWeatherData Data;

		struct _WeatherNode * Next = nullptr;
		struct _WeatherNode * Prev = nullptr;
	}GWeatherNode;

	typedef struct _WeatherList//���� ����Ʈ
	{
		int NumOfData = 0;

		GWeatherNode * Cur;
		GWeatherNode * Head;
		GWeatherNode * Tail;
	}GWeatherList;

	typedef struct _AnimalData //���� ������
	{
		unsigned char AnimalCode = 0;
		unsigned char Health = 0;
		unsigned char Damage = 0;

		unsigned char ChunkX = 0;
		unsigned char ChunkY = 0;
		unsigned char TileX = 0;
		unsigned char TileY = 0;
	}GAnimalData;

	typedef struct _AnimalDataNode //���� ������ ���
	{
		GAnimalData Data;

		struct _AnimalDataNode * Next = nullptr;
		struct _AnimalDataNode * Prev = nullptr;
	}GAnimalDataNode;

	typedef struct _AnimalDataList //���� ������ ����Ʈ
	{
		int NumOfData = 0;

		GAnimalDataNode * Cur = nullptr;
		GAnimalDataNode * Head = nullptr;
		GAnimalDataNode * Tail = nullptr;
	}GAnimalDataList;

	typedef struct _FieldAnimalDataList //�ʵ� ���� ����Ʈ
	{
		/*
		0 ���� �ϴ�
		1 ������ �ϴ�
		2 ���� ���
		3 ������ ���
		*/

		GAnimalDataList * Lo[4];

		unsigned char x = 0;
		unsigned char y = 0;

		bool bLo[4];
	}GFieldAnimalList;

	typedef struct _ItemDataNode //������ ���
	{
		GItemData Data;

		struct _ItemDataNode * Next = nullptr;
		struct _ItemDataNode * Prev = nullptr;
	}GItemDataNode;

	typedef struct _ItemDataList //������ ����Ʈ
	{
		int NumOfData = 0;

		GItemDataNode * Cur = nullptr;
		GItemDataNode * Head = nullptr;
		GItemDataNode * Tail = nullptr;
	}GItemDataList;

	typedef struct _FieldItemDataList //�ʵ� ������ ����Ʈ
	{
		/*
		0 ���� �ϴ�
		1 ������ �ϴ�
		2 ���� ���
		3 ������ ���
		*/
		GItemDataList * Lo[4];

		unsigned char x = 0;
		unsigned char y = 0;

		bool bLo[4];
	}GFieldItemList;

	typedef struct _ItemSpawnNode //������ ���� ������ ���� ���
	{
		int Data;

		struct _ItemSpawnNode * next;
	}GItemSpawnNode;

	typedef struct _ItemSpawnQueue //������ ���� ������ ���� ť
	{
		GItemSpawnNode * Front;
		GItemSpawnNode * Rear;
	}GItemSpawnQueue;

	//���� ����Ʈ ����
	void WeatherListInit(GWeatherList * _list);
	GWeatherNode * WeatherListAdd(GWeatherList * _list, GWeatherData _data);	//����Ʈ�� ��� �߰�

	bool WeatherListFirst(GWeatherList * _list);
	bool WeatherListFirst(GWeatherList * _list, GWeatherData * _data);			//����Ʈ�� �� �� ���� �̵�
	bool WeatherListLast(GWeatherList * _list, GWeatherData * _data);			//����Ʈ�� �� �� ���� �̵�
	bool WeatherListNext(GWeatherList * _list, GWeatherData * _data);			//���õ� ����� ���� ���� �̵�
	bool WeatherListPrev(GWeatherList * _list, GWeatherData * _data);			//���õ� ����� �� ���� �̵�
	int ListCount(GWeatherList * _list);										//����Ʈ�� �ִ� ����� ���� ��ȯ
	GWeatherNode * GetWeatherNode(GWeatherList * _list);						//����Ʈ�� �ִ� ���� ��� ��ȯ
	void RemoveNode(GWeatherList * _list, GWeatherNode * _node);				//����Ʈ���� ��� ����

	//���� ����Ʈ ����
	void AnimalDataListInit(GAnimalDataList * _list);
	GAnimalDataNode * AnimalDataListAdd(GAnimalDataList * _list, GAnimalData _data);
	void AnimalDataListAdd(GAnimalDataList * _list, GAnimalDataNode * _node);
	GAnimalDataNode * CreateAnimalDataNode(GAnimalData _data);

	bool AnimalDataListFirst(GAnimalDataList * _list);
	bool AnimalDataListLast(GAnimalDataList * _list);
	GAnimalDataNode *  AnimalDataListPick(GAnimalDataList * _list);
	GAnimalDataNode *  AnimalDataListNext(GAnimalDataList * _list);
	GAnimalDataNode *  AnimalDataListPrev(GAnimalDataList * _list);
	int ListCount(GAnimalDataList * _list);
	GAnimalDataNode * GetAnimalDataNode(GAnimalDataList * _list);
	void RemoveNode(GAnimalDataList * _list, GAnimalDataNode * _node);

	//������ ���� ���� ť ����
	void ItemSpawnQueueInit(GItemSpawnQueue * _queue);
	bool ItemSpawnQueueIsEmpty(GItemSpawnQueue * _queue);	//ť�� ����ִ��� Ȯ��

	void ItemSpawnEnQueue(GItemSpawnQueue * _queue, int _data);		//ť�� �� �� ������ �߰�
	int ItemSpawnDeQueue(GItemSpawnQueue * _queue);					//ť�� �� �� ������ ���� �� ��ȯ
	void ItemSpawnDeleteQueue(GItemSpawnQueue * _queue, int _data); //ť�� �ش��ϴ� ������ �˻� �� ����
	//GItemSpawnNode ItemSpawnNextQueue(GItemSpawnNode * _node);		//��忡 �ش��ϴ� ���� ��� ��ȯ


	//������ ����Ʈ ����
	void ItemDataListInit(GItemDataList * _list);
	GItemDataNode * ItemDataListAdd(GItemDataList * _list, GItemData _data);	//����Ʈ�� ��� �߰�
	void ItemDataListAdd(GItemDataList * _list, GItemDataNode * _data);			//����Ʈ�� ��� �߰�
	GItemDataNode * CreateItemDataNode(GItemData _data);

	bool ItemDataListFirst(GItemDataList * _list);
	bool ItemDataListLast(GItemDataList * _list);
	bool ItemDataListPick(GItemDataList * _list, GItemData * _data);
	bool ItemDataListNext(GItemDataList * _list, GItemData * _data);
	bool ItemDataListPrev(GItemDataList * _list, GItemData * _data);
	int ListCount(GItemDataList * _list);
	GItemDataNode * GetItemDataNode(GItemDataList * _list);
	void RemoveNode(GItemDataList * _list, GItemDataNode * _node);
}
