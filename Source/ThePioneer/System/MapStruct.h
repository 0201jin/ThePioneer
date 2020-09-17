// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "DataTable/ItemDataStruct.h"

#include "CoreMinimal.h"

/*새로운 오브젝트 생성시 변경*/
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

		unsigned char Obj_Max_Grow = 0; //최대 성장
		unsigned char Obj_Mat_Num = 0; //머트리얼 넘버
		bool Can_Dispose = false;
		bool Can_Install[5] = { false, false, false, false, false }; //물 사막 초원 눈 얼음

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

	//ItemCode를 값으로 넣는다.
	//오브젝트를 초기에 배치할때 사용
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
		unsigned char ItemCode = 0;			//아이템 코드
		unsigned char ItemAidCode = 0;		//보조 아이템 코드
		unsigned char Amount = 0;			//아이템 수량 혹은 내구도

		unsigned char Attack = 0;			//아이템의 공격력
		unsigned char Sheild = 0;			//아이템의 방어력
		unsigned char Distance = 0;			//아이템의 사거리
		unsigned char Health = 0;			//아이템의 체력

		int x = 0;							//아이템의 위치(길바닥)
		int y = 0;							//아이템의 위치(길바닥)

		_ItemDataNode * NodeAdr;			//노드의 주솟값
	}GItemData;

	typedef struct _Chunk //청크 구조체
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

	typedef struct _SeedChunk //청크 시드 데이터
	{
		unsigned char Seed[257][257] = { 0 };
	}GSeedChunk;

	typedef struct _CloudChunk //구름 청크 데이터
	{
		unsigned char Tile[256][256] = { 0 };
		int x, y;
	}GCloudChunk;

	typedef struct _WeatherData //날씨 데이터
	{
		unsigned char Rain;
		int x, y; //중심
		int sx, sy; //지름
		float time; //이벤트 남은 시간 (for문 실행 횟수)
	}GWeatherData;

	typedef struct _WeatherNode //날씨 노드
	{
		GWeatherData Data;

		struct _WeatherNode * Next = nullptr;
		struct _WeatherNode * Prev = nullptr;
	}GWeatherNode;

	typedef struct _WeatherList//날씨 리스트
	{
		int NumOfData = 0;

		GWeatherNode * Cur;
		GWeatherNode * Head;
		GWeatherNode * Tail;
	}GWeatherList;

	typedef struct _AnimalData //동물 데이터
	{
		unsigned char AnimalCode = 0;
		unsigned char Health = 0;
		unsigned char Damage = 0;

		unsigned char ChunkX = 0;
		unsigned char ChunkY = 0;
		unsigned char TileX = 0;
		unsigned char TileY = 0;
	}GAnimalData;

	typedef struct _AnimalDataNode //동물 데이터 노드
	{
		GAnimalData Data;

		struct _AnimalDataNode * Next = nullptr;
		struct _AnimalDataNode * Prev = nullptr;
	}GAnimalDataNode;

	typedef struct _AnimalDataList //동물 데이터 리스트
	{
		int NumOfData = 0;

		GAnimalDataNode * Cur = nullptr;
		GAnimalDataNode * Head = nullptr;
		GAnimalDataNode * Tail = nullptr;
	}GAnimalDataList;

	typedef struct _FieldAnimalDataList //필드 동물 리스트
	{
		/*
		0 왼쪽 하단
		1 오른쪽 하단
		2 왼쪽 상단
		3 오른쪽 상단
		*/

		GAnimalDataList * Lo[4];

		unsigned char x = 0;
		unsigned char y = 0;

		bool bLo[4];
	}GFieldAnimalList;

	typedef struct _ItemDataNode //아이템 노드
	{
		GItemData Data;

		struct _ItemDataNode * Next = nullptr;
		struct _ItemDataNode * Prev = nullptr;
	}GItemDataNode;

	typedef struct _ItemDataList //아이템 리스트
	{
		int NumOfData = 0;

		GItemDataNode * Cur = nullptr;
		GItemDataNode * Head = nullptr;
		GItemDataNode * Tail = nullptr;
	}GItemDataList;

	typedef struct _FieldItemDataList //필드 아이템 리스트
	{
		/*
		0 왼쪽 하단
		1 오른쪽 하단
		2 왼쪽 상단
		3 오른쪽 상단
		*/
		GItemDataList * Lo[4];

		unsigned char x = 0;
		unsigned char y = 0;

		bool bLo[4];
	}GFieldItemList;

	typedef struct _ItemSpawnNode //아이템 스폰 순서에 대한 노드
	{
		int Data;

		struct _ItemSpawnNode * next;
	}GItemSpawnNode;

	typedef struct _ItemSpawnQueue //아이템 스폰 순서에 대한 큐
	{
		GItemSpawnNode * Front;
		GItemSpawnNode * Rear;
	}GItemSpawnQueue;

	//날씨 리스트 관리
	void WeatherListInit(GWeatherList * _list);
	GWeatherNode * WeatherListAdd(GWeatherList * _list, GWeatherData _data);	//리스트에 노드 추가

	bool WeatherListFirst(GWeatherList * _list);
	bool WeatherListFirst(GWeatherList * _list, GWeatherData * _data);			//리스트의 맨 앞 노드로 이동
	bool WeatherListLast(GWeatherList * _list, GWeatherData * _data);			//리스트의 맨 뒤 노드로 이동
	bool WeatherListNext(GWeatherList * _list, GWeatherData * _data);			//선택된 노드의 다음 노드로 이동
	bool WeatherListPrev(GWeatherList * _list, GWeatherData * _data);			//선택된 노드의 전 노드로 이동
	int ListCount(GWeatherList * _list);										//리스트에 있는 노드의 개수 반환
	GWeatherNode * GetWeatherNode(GWeatherList * _list);						//리스트에 있는 현재 노드 반환
	void RemoveNode(GWeatherList * _list, GWeatherNode * _node);				//리스트에서 노드 삭제

	//동물 리스트 관리
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

	//아이템 스폰 순서 큐 관리
	void ItemSpawnQueueInit(GItemSpawnQueue * _queue);
	bool ItemSpawnQueueIsEmpty(GItemSpawnQueue * _queue);	//큐가 비어있는지 확인

	void ItemSpawnEnQueue(GItemSpawnQueue * _queue, int _data);		//큐에 맨 앞 데이터 추가
	int ItemSpawnDeQueue(GItemSpawnQueue * _queue);					//큐에 맨 앞 데이터 삭제 후 반환
	void ItemSpawnDeleteQueue(GItemSpawnQueue * _queue, int _data); //큐에 해당하는 데이터 검색 후 삭제
	//GItemSpawnNode ItemSpawnNextQueue(GItemSpawnNode * _node);		//노드에 해당하는 다음 노드 반환


	//아이템 리스트 관리
	void ItemDataListInit(GItemDataList * _list);
	GItemDataNode * ItemDataListAdd(GItemDataList * _list, GItemData _data);	//리스트에 노드 추가
	void ItemDataListAdd(GItemDataList * _list, GItemDataNode * _data);			//리스트에 노드 추가
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
