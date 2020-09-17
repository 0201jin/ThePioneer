// Fill out your copyright notice in the Description page of Project Settings.

#include "MapGenerator.h"
#include "Engine.h"

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>

using namespace std;

//랜덤으로 시드에 맞는 값을 출력(최대 254)
inline static int random(int range)
{
	srand(range);
	return (rand() % 255);
}

MapGenerator::MapGenerator(AInGameLevel * _InGameLevel)
{
	InGameLevel = _InGameLevel;
	MyGameInstance = InGameLevel->MyGameInstance;

	WeatherDataList = new GWeatherList;

	ObjectHead = new GObjectData;

	WeatherListInit(WeatherDataList);

	for (int x = 0; x < 32; x++)
	{
		for (int y = 0; y < 32; y++)
		{
			AnimalChunkData[x][y].Lo[0] = new GAnimalDataList;
			AnimalDataListInit(AnimalChunkData[x][y].Lo[0]);

			AnimalChunkData[x][y].Lo[1] = new GAnimalDataList;
			AnimalDataListInit(AnimalChunkData[x][y].Lo[1]);

			AnimalChunkData[x][y].Lo[2] = new GAnimalDataList;
			AnimalDataListInit(AnimalChunkData[x][y].Lo[2]);

			AnimalChunkData[x][y].Lo[3] = new GAnimalDataList;
			AnimalDataListInit(AnimalChunkData[x][y].Lo[3]);

			AnimalChunkData[x][y].x = x;
			AnimalChunkData[x][y].y = y;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("%d"), sizeof(GItemData));
	UE_LOG(LogTemp, Warning, TEXT("%d"), sizeof(GObjectData));
	UE_LOG(LogTemp, Warning, TEXT("MapGenerator On"));
}

MapGenerator::~MapGenerator()
{
}

//시드에 맞는 맵을 생성한다.
bool MapGenerator::GenerateMap(int _Seed)
{
	DivisionSeed(_Seed);
	//Diamond Square Algorithm을 이용해서 맵 청크 시드를 생성
	DiamondSquareAlgorithm(ChunkSeed, MAP_SIZE);

	SetSeeLevel();

	CreateChunk();

	UE_LOG(LogTemp, Warning, TEXT("Generate Map Done!"));
	//MakeChunkToImage();

	return true;
}

//위치에 맞는 청크를 로드한다.(청크가 바꿀때마다 호출)
void MapGenerator::LoadChunk(int _x, int _y)
{
	UE_LOG(LogTemp, Warning, TEXT("LoadChunk x y %d %d"), _x, _y);
	for (int col = _x - 1; col <= _x + 1; col++)
	{
		for (int row = _y - 1; row <= _y + 1; row++)
		{
			if (col >= 0 && row >= 0 && col < 32 && row < 32)
			{
				StreamChunkData[col - _x + 1][row - _y + 1] = ChunkData[col][row];
			}
		}
	}
}

GChunk(*MapGenerator::GetStreamChunk())[3]
{
	return StreamChunkData;
}

//구름 청크 만들기
void MapGenerator::CreateCloudChunk(int _x, int _y, int _sx, int _sy)
{
	int sx = _sx * 0.5;
	int sy = _sy * 0.5;

	for (int x = _x - sx; x < _x + sx; x++)
	{
		for (int y = _y - sy; y < _y + sy; y++)
		{
			int ChunkLoX = x / 256;
			int ChunkLoY = y / 256;

			if ((x >= 0 && y >= 0) && (x < 8192 && y < 8192))
			{
				int TileX = x % 256;
				int TileY = y % 256;

				CloudChunkData[ChunkLoX][ChunkLoY].x = ChunkLoX;
				CloudChunkData[ChunkLoX][ChunkLoY].y = ChunkLoY;
				CloudChunkData[ChunkLoX][ChunkLoY].Tile[TileX][TileY] += 1;
			}
		}
	}
}

//구름 삭제
void MapGenerator::DeleteCloudChunk(int _x, int _y, int _sx, int _sy)
{
	int sx = _sx * 0.5;
	int sy = _sy * 0.5;

	for (int x = _x - sx; x < _x + sx; x++)
	{
		for (int y = _y - sy; y < _y + sy; y++)
		{
			int ChunkLoX = x / 256;
			int ChunkLoY = y / 256;

			if ((x >= 0 && y >= 0) && (x < 8192 && y < 8192))
			{
				int TileX = x % 256;
				int TileY = y % 256;

				CloudChunkData[ChunkLoX][ChunkLoY].Tile[TileX][TileY] -= 1;
			}
		}
	}
}

//구름 실시간 청크 제작
void MapGenerator::LoadCloudChunk(int _x, int _y)
{
	UE_LOG(LogTemp, Warning, TEXT("LoadCloudChunk x y %d %d"), _x, _y);
	for (int col = _x - 1; col <= _x + 1; col++)
	{
		for (int row = _y - 1; row <= _y + 1; row++)
		{
			if (col >= 0 && row >= 0 && col < 32 && row < 32)
			{
				StreamCloud[col - _x + 1][row - _y + 1] = CloudChunkData[col][row];
			}
		}
	}
}

//구름 실시간 청크 반환
GCloudChunk(*MapGenerator::GetStreamCloudChunk())[3]
{
	return StreamCloud;
}

//오브젝트 배치
void MapGenerator::DisposeObjectChunk(int _x, int _y, GObjectData _data)
{
	int ChunkLoX = _x / 256;
	int ChunkLoY = _y / 256;

	if ((_x >= 0 && _y >= 0) && (_x < 8192 && _y < 8192))
	{
		int TileX = _x % 256;
		int TileY = _y % 256;

		if (ObjectChunkData[ChunkLoX][ChunkLoY].Tile[TileX][TileY] == 0)
		{
			GObjectData * Object = new GObjectData;
			*(Object) = _data;

			ObjectChunkData[ChunkLoX][ChunkLoY].Tile[TileX][TileY] = Object;

			ObjCount++;
		}
	}
}

//오브젝트 삭제
void MapGenerator::DeleteObjectChunk(int _x, int _y)
{
	int ChunkLoX = _x / 256;
	int ChunkLoY = _y / 256;

	if ((_x >= 0 && _y >= 0) && (_x < 8192 && _y < 8192))
	{
		int TileX = _x % 256;
		int TileY = _y % 256;

		if (ObjectChunkData[ChunkLoX][ChunkLoY].Tile[TileX][TileY] != 0)
		{
			delete(ObjectChunkData[ChunkLoX][ChunkLoY].Tile[TileX][TileY]);
			ObjCount--;
		}

		ObjectChunkData[ChunkLoX][ChunkLoY].Tile[TileX][TileY] = 0;
	}
}

//오브젝트 변경
void MapGenerator::ChangeObjectData(int _x, int _y, GObjectData _data)
{
	int ChunkLoX = _x / 256;
	int ChunkLoY = _y / 256;

	if ((_x >= 0 && _y >= 0) && (_x < 8192 && _y < 8192))
	{
		int TileX = _x % 256;
		int TileY = _y % 256;

		*(ObjectChunkData[ChunkLoX][ChunkLoY].Tile[TileX][TileY]) = _data;

		//ObjectChunkData[ChunkLoX][ChunkLoY].Tile[TileX][TileY] = _data;
	}
}

//오브젝트 실시간 청크 제작
void MapGenerator::LoadObjectChunk(int _x, int _y)
{
	UE_LOG(LogTemp, Warning, TEXT("LoadObjectChunk x y %d %d"), _x, _y);
	for (int col = _x - 1; col <= _x + 1; col++)
	{
		for (int row = _y - 1; row <= _y + 1; row++)
		{
			if (col >= 0 && row >= 0 && col < 32 && row < 32)
			{
				StreamObject[col - _x + 1][row - _y + 1] = ObjectChunkData[col][row];
			}
		}
	}
}

//오브젝트 실시간 청크 반환
GObjectChunk(*MapGenerator::GetStreamObjectChunk())[3]
{
	return StreamObject;
}

GObjectData * MapGenerator::GetObjectData(int _x, int _y)
{
	int ChunkLoX = _x / 256;
	int ChunkLoY = _y / 256;

	if ((_x >= 0 && _y >= 0) && (_x < 8192 && _y < 8192))
	{
		int TileX = _x % 256;
		int TileY = _y % 256;

		if (ObjectChunkData[ChunkLoX][ChunkLoY].Tile[TileX][TileY] != 0)
		{
			return ObjectChunkData[ChunkLoX][ChunkLoY].Tile[TileX][TileY];
		}
	}

	return nullptr;
}

bool MapGenerator::CheckObjChunk(int _x, int _y)
{
	int ChunkX = _x / 256;
	int ChunkY = _y / 256;

	int TileX = _x % 256;
	int TileY = _y % 256;

	if (ObjectChunkData[ChunkX][ChunkY].Tile[TileX][TileY] != 0)
		return false;

	return true;
}

//날씨가 오브젝트에 영향을 주는 함수
void MapGenerator::WeatherToObject(GWeatherData _data, float _DeltaSecond)
{
	int sx = _data.sx * 0.5;
	int sy = _data.sy * 0.5;

	for (int x = _data.x - sx; x < _data.x + sx; x++)
	{
		for (int y = _data.y - sy; y < _data.y + sy; y++)
		{
			int ChunkLoX = x / 256;
			int ChunkLoY = y / 256;

			int TileX = x % 256;
			int TileY = y % 256;

			GObjectData * Object = (ObjectChunkData[ChunkLoX][ChunkLoY].Tile[TileX][TileY]);

			if (Object)
			{
				if (Object->Grow < MyGameInstance->ObjDataBase[Object->Code][Object->AidCode].Obj_Max_Grow)
				{
					Object->FExp += 1 + _DeltaSecond;

					if (Object->FExp >= 255)
					{
						Object->Exp++;
						Object->FExp = 0;
					}

					if (Object->Exp >= 255)
					{
						Object->Grow++;
						Object->Exp = 0;

						InGameLevel->ChangeObjChunk(x, y, *Object);
					}
				}
			}
		}
	}
}

//청크의 시드를 생성하고 시드를 청크로 바꾼다.
void MapGenerator::CreateChunk()
{
	UE_LOG(LogTemp, Warning, TEXT("Create Chunk"));
	for (int col = 0; col < MAP_SIZE; col++)
	{
		for (int row = 0; row < MAP_SIZE; row++)
		{
			SeedChunkData[col][row].Seed[0][0] = ChunkSeed[col][row];
			SeedChunkData[col][row].Seed[CHUNK_SIZE][0] = ChunkSeed[col + 1][row];
			SeedChunkData[col][row].Seed[0][CHUNK_SIZE] = ChunkSeed[col][row + 1];
			SeedChunkData[col][row].Seed[CHUNK_SIZE][CHUNK_SIZE] = ChunkSeed[col + 1][row + 1];
		}
	}

	CDiamondSquareAlgorithm(CHUNK_SIZE);

	for (int col = 0; col < MAP_SIZE; col++)
	{
		for (int row = 0; row < MAP_SIZE; row++)
		{
			ChunkData[col][row] = CreateSeedToTile(SeedChunkData[col][row].Seed, col, row);
		}
	}

	SetFinalPlayerLo();

	PX *= 100;
	PY *= 100;
}

//맵을 이미지화 시킨다.
void MapGenerator::MakeChunkToImage()
{
	FILE * fp = fopen("ChunkPic.ppm", "wt");
	if (fp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("File Open Fail"));
		return;
	}

	fputs("P3 ", fp);
	//fputs("\r\n", fp);
	fputs("8192 8192", fp);
	//fputs("\r\n", fp);
	fputs(" 255", fp);
	fputs("\r\n", fp);

	for (int y = MAP_SIZE - 1; y >= 0; y--)
	{
		for (int row = CHUNK_SIZE - 1; row >= 0; row--)
		{
			for (int x = 0; x < MAP_SIZE; x++)
			{
				for (int col = 0; col < CHUNK_SIZE; col++)
				{
					int rgb = ChunkData[y][x].Tile[row][col];
					string str = "0 " + to_string(rgb) + " 0";
					fputs(str.c_str(), fp);
					fputs("\r\n", fp);
				}
			}
		}
	}
	fclose(fp);
}

void MapGenerator::CreateAnimal()
{
	int RandCount = (rand() % 5000) + 5000;

	for (int i = 0; i < RandCount; i++)
	{
		int ChunkX = rand() % 32;
		int ChunkY = rand() % 32;
		int TileX = rand() % 256;
		int TileY = rand() % 256;

		int Tile = ChunkData[ChunkX][ChunkY].Tile[TileX][TileY];

		if (MyGameInstance->HabitatCount[Tile] > 0)
		{
			int UDLR = ((TileX / 128) * 2) + (TileY / 128);

			int RandHabitat = rand() % MyGameInstance->HabitatCount[Tile];

			GAnimalData Data;
			Data.AnimalCode = MyGameInstance->Habitat[Tile][RandHabitat];
			Data.ChunkX = ChunkX;
			Data.ChunkY = ChunkY;
			Data.TileX = TileX;
			Data.TileY = TileY;
			Data.Damage = MyGameInstance->AnimalDataBase[Data.AnimalCode].Damage;
			Data.Health = MyGameInstance->AnimalDataBase[Data.AnimalCode].Health;

			AnimalDataListAdd(AnimalChunkData[ChunkX][ChunkY].Lo[UDLR], Data);
		}
	}
}

void MapGenerator::LoadAnimalList(int _x, int _y)
{
	for (int col = _x - 1; col <= _x + 1; col++)
	{
		for (int row = _y - 1; row <= _y + 1; row++)
		{
			if (col >= 0 && row >= 0 && col < 32 && row < 32)
			{
				StreamAnimalChunkData[col - _x + 1][row - _y + 1] = AnimalChunkData[col][row];
			}
		}
	}
}

GFieldAnimalList(*MapGenerator::GetStreamFieldAnimalList())[3]
{
	return StreamAnimalChunkData;
}

void MapGenerator::SetAnimalChunk(GFieldAnimalList(*_AnimalData)[3])
{
	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			int ChunkX = _AnimalData[x][y].x;
			int ChunkY = _AnimalData[x][y].y;
			AnimalChunkData[ChunkX][ChunkY] = _AnimalData[x][y];
		}
	}
}

//날씨에 대한 이벤트를 만드는 함수다.
void MapGenerator::CreateWeatherEvent(int _x, int _y, int _sx, int _sy, bool _Rain)
{
	//날씨 연결 리스트를 추가하는 코드
	GWeatherData _data;
	_data.Rain = _Rain;
	_data.x = _x;
	_data.y = _y;
	_data.sx = _sx;
	_data.sy = _sy;

	WeatherListAdd(WeatherDataList, _data);
}

//시드를 쪼개고 재분배한다.
void MapGenerator::DivisionSeed(int _Seed)
{
	for (int i = 0; i < 33; i++)
	{
		if (i % 4 == 0)
		{
			int Sd = _Seed;
			ChunkSeed[0][0] <<= 1;
			Sd <<= 32 - i;
			Sd >>= 31;
			ChunkSeed[0][0] |= Sd;
		}

		else if (i % 4 == 1)
		{
			int Sd = _Seed;
			ChunkSeed[32][0] <<= 1;
			Sd <<= 32 - i;
			Sd >>= 31;
			ChunkSeed[32][0] |= Sd;
		}

		else if (i % 4 == 2)
		{
			int Sd = _Seed;
			ChunkSeed[32][32] <<= 1;
			Sd <<= 32 - i;
			Sd >>= 31;
			ChunkSeed[32][32] |= Sd;
		}

		else if (i % 4 == 3)
		{
			int Sd = _Seed;
			ChunkSeed[0][32] <<= 1;
			Sd <<= 32 - i;
			Sd >>= 31;
			ChunkSeed[0][32] |= Sd;
		}
	}
	Seed = _Seed;

	UE_LOG(LogTemp, Warning, TEXT("Division Seed!"));

	/*ChunkSeed[0][0] = _Seed;

	_Seed >>= 8;
	ChunkSeed[32][32] = _Seed;

	_Seed >>= 8;
	ChunkSeed[0][32] = _Seed;

	_Seed >>= 8;
	ChunkSeed[32][0] = _Seed;*/
}

//해수면과 타일 종류에 대한 기준을 잡는다.
void MapGenerator::SetSeeLevel()
{
	int aa = 0;
	for (int y = 0; y < MAP_SEED_SIZE; y++)
	{
		for (int x = 0; x < MAP_SEED_SIZE; x++)
		{
			SeeLevel += ChunkSeed[x][y];
			aa++;
		}
	}

	SeeLevel /= (aa);
	TileLevel = (255 + SeeLevel) / 2;

	UE_LOG(LogTemp, Warning, TEXT("Set SeeLevel!"));
	//SeeLevel = (ChunkSeed[0][0] + ChunkSeed[MAP_SIZE][0] + ChunkSeed[0][MAP_SIZE] + ChunkSeed[MAP_SIZE][MAP_SIZE] + random(Seed)) / 5;
}

//맵 시드를 Diamond-Square로 만든다.
void MapGenerator::DiamondSquareAlgorithm(unsigned char _ChunkSeed[MAP_SEED_SIZE][MAP_SEED_SIZE], int size)
{
	int Half = size / 2;

	if (Half < 1)
		return;

	//Square Step
	for (int x = Half; x < (MAP_SEED_SIZE); x += size)
		for (int y = Half; y < (MAP_SEED_SIZE); y += size)
			SquareStep(_ChunkSeed, x % (MAP_SEED_SIZE), y % (MAP_SEED_SIZE), Half);

	//Diamond Step
	int Col = 0;

	for (int x = 0; x < (MAP_SEED_SIZE); x += Half)
	{
		Col++;

		if (Col % 2 == 1)
			for (int y = Half; y < (MAP_SEED_SIZE); y += size)
				DiamondStep(_ChunkSeed, x % (MAP_SEED_SIZE), y % (MAP_SEED_SIZE), Half);

		else
			for (int y = 0; y < (MAP_SEED_SIZE); y += size)
				DiamondStep(_ChunkSeed, x % (MAP_SEED_SIZE), y % (MAP_SEED_SIZE), Half);
	}

	DiamondSquareAlgorithm(_ChunkSeed, Half);
}

//맵에 대한 DiamondStep 알고리즘이다.
void MapGenerator::DiamondStep(unsigned char _ChunkSeed[MAP_SEED_SIZE][MAP_SEED_SIZE], int x, int y, int dis)
{
	int count = 0;
	int avg = 0;

	if (x - dis >= 0)
	{
		avg += _ChunkSeed[x - dis][y];
		count++;
	}
	if (x + dis < MAP_SEED_SIZE)
	{
		avg += _ChunkSeed[x + dis][y];
		count++;
	}
	if (y - dis >= 0)
	{
		avg += _ChunkSeed[x][y - dis];
		count++;
	}
	if (y + dis < MAP_SEED_SIZE)
	{
		avg += _ChunkSeed[x][y + dis];
		count++;
	}
	avg /= count;

	avg += ((random(dis + x + y + avg)) / 4) - 32;

	int A, B, Result, Want = 0;

	Want = 255 - avg;
	A = avg - Want;
	B = A * ((dis / 16));
	Result = avg - B;

	_ChunkSeed[x][y] = avg;
}

//맵에 대한 SquareStep을 사용한다.
void MapGenerator::SquareStep(unsigned char _ChunkSeed[MAP_SEED_SIZE][MAP_SEED_SIZE], int x, int y, int dis)
{
	int count = 0;
	int avg = 0;

	if (x - dis >= 0 && y - dis >= 0)
	{
		avg += _ChunkSeed[x - dis][y - dis];
		count++;
	}
	if (x - dis >= 0 && y + dis < MAP_SEED_SIZE)
	{
		avg += _ChunkSeed[x - dis][y + dis];
		count++;
	}
	if (x + dis < MAP_SEED_SIZE && y - dis >= 0)
	{
		avg += _ChunkSeed[x + dis][y - dis];
		count++;
	}
	if (x + dis < MAP_SEED_SIZE && y + dis < MAP_SEED_SIZE)
	{
		avg += _ChunkSeed[x + dis][y + dis];
		count++;
	}
	avg /= count;

	avg += ((random(dis + x + y + avg)) / 4) - 32;

	int A, B, Result, Want = 0;

	Want = 255 - avg;
	A = avg - Want;
	B = A * ((dis / 16));
	Result = avg - B;

	_ChunkSeed[x][y] = Result;
}

//청크에 대한 Diamond-Square 알고리즘을 사용한다.
void MapGenerator::CDiamondSquareAlgorithm(int size)
{
	int Half = size / 2;

	if (Half < 1)
		return;

	//Square Step
	for (int col = 0; col < MAP_SIZE; col++)
		for (int row = 0; row < MAP_SIZE; row++)
			for (int x = Half; x < (CHUNK_SEED_SIZE); x += size)
				for (int y = Half; y < (CHUNK_SEED_SIZE); y += size)
					CSquareStep(SeedChunkData[col][row].Seed, x % (CHUNK_SEED_SIZE), y % (CHUNK_SEED_SIZE), Half);

	//Diamond Step
	for (int col = 0; col < MAP_SIZE; col++)
		for (int row = 0; row < MAP_SIZE; row++)
		{
			int Col = 0;

			for (int x = 0; x < (CHUNK_SEED_SIZE); x += Half)
			{
				Col++;

				if (Col % 2 == 1)
					for (int y = Half; y < (CHUNK_SEED_SIZE); y += size)
						CDiamondStep(SeedChunkData[col][row].Seed, x % (CHUNK_SEED_SIZE), y % (CHUNK_SEED_SIZE), Half, col, row);

				else
					for (int y = 0; y < (CHUNK_SEED_SIZE); y += size)
						CDiamondStep(SeedChunkData[col][row].Seed, x % (CHUNK_SEED_SIZE), y % (CHUNK_SEED_SIZE), Half, col, row);
			}
		}

	CDiamondSquareAlgorithm(Half);
}

//청크에 대한 DiamondStep을 사용한다.
void MapGenerator::CDiamondStep(unsigned char _ChunkDataSeed[CHUNK_SEED_SIZE][CHUNK_SEED_SIZE], int x, int y, int dis, int col, int row)
{
	int count = 0;
	int avg = 0;

	//금을 넘었으면 다른 청크의 해당 데이터를 가져오는 else 문 만들기
	if (x - dis >= 0)
	{
		avg += _ChunkDataSeed[x - dis][y];
		count++;
	}
	else if (col - 1 >= 0)
	{
		avg += SeedChunkData[col - 1][row].Seed[CHUNK_SIZE + (x - dis)][y];
		count++;
	}

	if (x + dis < CHUNK_SEED_SIZE)
	{
		avg += _ChunkDataSeed[x + dis][y];
		count++;
	}
	else if (col + 1 < MAP_SIZE)
	{
		avg += SeedChunkData[col + 1][row].Seed[(x + dis) - CHUNK_SIZE][y];
		count++;
	}

	if (y - dis >= 0)
	{
		avg += _ChunkDataSeed[x][y - dis];
		count++;
	}
	else if (row - 1 >= 0)
	{
		avg += SeedChunkData[col][row - 1].Seed[x][CHUNK_SIZE + (y - dis)];
		count++;
	}

	if (y + dis < CHUNK_SEED_SIZE)
	{
		avg += _ChunkDataSeed[x][y + dis];
		count++;
	}
	else if (row + 1 < MAP_SIZE)
	{
		avg += SeedChunkData[col][row + 1].Seed[x][(y + dis) - CHUNK_SIZE];
		count++;
	}

	avg /= count;

	int A, B, Result, Want = 0;

	Want = 255 - avg;
	A = avg - Want;
	B = A * ((dis / 255));
	Result = avg - B;

	_ChunkDataSeed[x][y] = Result;
}

//청크에 대한 SquareStep을 사용한다.
void MapGenerator::CSquareStep(unsigned char _ChunkDataSeed[CHUNK_SEED_SIZE][CHUNK_SEED_SIZE], int x, int y, int dis)
{
	int count = 0;
	int avg = 0;

	if (x - dis >= 0 && y - dis >= 0)
	{
		avg += _ChunkDataSeed[x - dis][y - dis];
		count++;
	}
	if (x - dis >= 0 && y + dis < CHUNK_SEED_SIZE)
	{
		avg += _ChunkDataSeed[x - dis][y + dis];
		count++;
	}
	if (x + dis < CHUNK_SEED_SIZE && y - dis >= 0)
	{
		avg += _ChunkDataSeed[x + dis][y - dis];
		count++;
	}
	if (x + dis < CHUNK_SEED_SIZE && y + dis < CHUNK_SEED_SIZE)
	{
		avg += _ChunkDataSeed[x + dis][y + dis];
		count++;
	}

	avg /= count;

	int A, B, Result, Want = 0;

	Want = 255 - avg;
	A = avg - Want;
	B = A * ((dis / 255));
	Result = avg - B;

	_ChunkDataSeed[x][y] = Result;
}

//청크 시드를 타일로 만든다.
GChunk MapGenerator::CreateSeedToTile(unsigned char _ChunkDataSeed[CHUNK_SEED_SIZE][CHUNK_SEED_SIZE], int _x, int _y)
{
	GChunk _ChunkData;
	_ChunkData.x = _x;
	_ChunkData.y = _y;

	int ChunkXData = (15 - _x);
	int Dist = (((SeeLevel / 2) + 128) - (SeeLevel / 2)) / 4;
	int SeeLevelHalf = SeeLevel / 2;

	ChunkXData *= 256;

	if (ChunkXData < 0)
	{
		for (int x = 0; x < CHUNK_SIZE; x++)
		{

			int TileLoData = 0;

			TileLoData = (ChunkXData - x) * 0.03125;
			TileLoData *= -1;

			for (int y = 0; y < CHUNK_SIZE; y++)
			{
				int result = 0;

				result += _ChunkDataSeed[x][y];
				result += _ChunkDataSeed[x + 1][y];
				result += _ChunkDataSeed[x][y + 1];
				result += _ChunkDataSeed[x + 1][y + 1];

				result /= 4;

				if (result <= SeeLevel)
					result = 0;

				else
				{
					//이곳에 위도를 기준으로 어떤 타일을 깔 것인지를 계산
					int Standard = (result / 2) + TileLoData;

					result = SetTile(result, Standard, SeeLevelHalf, Dist, x, y, _x, _y);
				}

				_ChunkData.Tile[x][y] = result;
			}
		}
	}

	else
	{
		for (int x = 0; x < CHUNK_SIZE; x++)
		{
			int TileLoData = 0;
			TileLoData = (ChunkXData - x) * 0.03125;

			for (int y = 0; y < CHUNK_SIZE; y++)
			{
				int result = 0;

				result += _ChunkDataSeed[x][y];
				result += _ChunkDataSeed[x + 1][y];
				result += _ChunkDataSeed[x][y + 1];
				result += _ChunkDataSeed[x + 1][y + 1];

				result /= 4;

				if (result <= SeeLevel)
					result = 0;

				else
				{
					//이곳에 위도를 기준으로 어떤 타일을 깔 것인지를 계산
					int Standard = (result / 2) + TileLoData;

					result = SetTile(result, Standard, SeeLevelHalf, Dist, x, y, _x, _y);
				}

				_ChunkData.Tile[x][y] = result;
			}
		}
	}

	return _ChunkData;
}

int MapGenerator::SetTile(int result, int Standard, int SeeLevelHalf, int Dist, int x, int y, int _x, int _y)
{
	if (Standard <= SeeLevelHalf + (Dist * 0.7))
	{
		//여기에 플레이어 배치 함수
		result = 1;
	}

	else if (Standard > SeeLevelHalf + (Dist * 0.7) && Standard <= SeeLevelHalf + (Dist * 3.7))
	{
		//여기에 플레이어 배치 함수
		result = 2;
	}

	else if (Standard > SeeLevelHalf + (Dist * 3.7) && Standard <= SeeLevelHalf + (Dist * 4.5))
	{
		//여기에 플레이어 배치 함수
		result = 3;
	}

	else if (Standard > SeeLevelHalf + (Dist * 4.5))
	{
		//여기에 플레이어 배치 함수
		result = 4;
	}

	SetObject(result, (_x * 256) + x, (_y * 256) + y);

	return result;
}

void MapGenerator::SetObject(int result, int x, int y)
{
	//UE_LOG(LogTemp, Warning, TEXT("%d"), result);
	
	if ((rand() % 100) > 97) //97은 확률
	{
		switch (result)
		{
		case 0: //물
			if (WATER_OBJ_COUNT > 0)
			{
				GObjectData data;
				int Code = rand() % WATER_OBJ_COUNT;
				data.AidCode = Water_Obj[Code];
				if (MyGameInstance->ObjDataBase[0][Water_Obj[Code]].Obj_Max_Grow > 0)
				{
					int Grow = rand() % (MyGameInstance->ObjDataBase[0][Water_Obj[Code]].Obj_Max_Grow);
					data.Grow = Grow;
				}

				DisposeObjectChunk(x, y, data);
			}
			break;

		case 1: //사막
			if (DESERT_OBJ_COUNT > 0)
			{
				GObjectData data;
				int Code = rand() % DESERT_OBJ_COUNT;
				data.AidCode = Desert_Obj[Code];
				if (MyGameInstance->ObjDataBase[0][Desert_Obj[Code]].Obj_Max_Grow > 0)
				{
					int Grow = rand() % (MyGameInstance->ObjDataBase[0][Desert_Obj[Code]].Obj_Max_Grow);
					data.Grow = Grow;
				}

				DisposeObjectChunk(x, y, data);
				SetPlayerLo(x, y, result);
			}
			break;

		case 2: //초원
			if (GRASS_OBJ_COUNT > 0)
			{
				GObjectData data;
				int Code = rand() % GRASS_OBJ_COUNT;
				data.AidCode = Grass_Obj[Code];
				if (MyGameInstance->ObjDataBase[0][Grass_Obj[Code]].Obj_Max_Grow > 0)
				{
					int Grow = rand() % (MyGameInstance->ObjDataBase[0][Grass_Obj[Code]].Obj_Max_Grow);
					data.Grow = Grow;
				}

				DisposeObjectChunk(x, y, data);
				SetPlayerLo(x, y, result);
			}
			break;

		case 3: //눈
			if (SNOW_OBJ_COUNT > 0)
			{
				GObjectData data;
				int Code = rand() % SNOW_OBJ_COUNT;
				data.AidCode = Snow_Obj[Code];
				if (MyGameInstance->ObjDataBase[0][Snow_Obj[Code]].Obj_Max_Grow > 0)
				{
					int Grow = rand() % (MyGameInstance->ObjDataBase[0][Snow_Obj[Code]].Obj_Max_Grow);
					data.Grow = Grow;
				}

				DisposeObjectChunk(x, y, data);
				SetPlayerLo(x, y, result);
			}
			break;

		case 4: //얼음
			if (ICE_OBJ_COUNT > 0)
			{
				GObjectData data;
				int Code = rand() % ICE_OBJ_COUNT;
				data.AidCode = Ice_Obj[Code];
				if (MyGameInstance->ObjDataBase[0][Ice_Obj[Code]].Obj_Max_Grow > 0)
				{
					int Grow = rand() % (MyGameInstance->ObjDataBase[0][Ice_Obj[Code]].Obj_Max_Grow);
					data.Grow = Grow;
				}

				DisposeObjectChunk(x, y, data);
				SetPlayerLo(x, y, result);
			}
			break;
		}
	}
}

void MapGenerator::SetPlayerLo(int _x, int _y, int _result)
{
	int x = _x / 256;
	int y = _y / 256;

	ChunkObjCount[x][y]++;

	if (ChunkObjCount[x][y] > MaxChunkCount)
	{
		MaxChunkX = x;
		MaxChunkY = y;

		MaxChunkCount = ChunkObjCount[x][y];
	}
}

void MapGenerator::SetFinalPlayerLo()
{
	for (int x = 0; x < 256; x++)
	{
		for (int y = 0; y < 256; y++)
		{
			if (ChunkData[MaxChunkX][MaxChunkY].Tile[x][y] != 0 &&
				ObjectChunkData[MaxChunkX][MaxChunkY].Tile[x][y] == 0)
			{
				PX = (MaxChunkX * 256) + x;
				PY = (MaxChunkY * 256) + y;

				return;
			}
		}
	}
}
