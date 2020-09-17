// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameLevel.h"
#include "Player/PlayerPawn.h"

#include "System/WeatherSystem.h"
#include "System/MapGenerator.h"
#include "Object/ObjectMgr.h"
#include "Item/ItemMgr.h"
#include "Animal/AniamlMgr.h"

MapGenerator * MapGen;
ObjectMgr * ObMgr;
WeatherSystem * WeatherSys;
ItemMgr * ItmMgr;
AniamlMgr * AnimalMgr;

AInGameLevel::AInGameLevel()
{
}

AInGameLevel::~AInGameLevel()
{
}

void AInGameLevel::LoadChunkToMapGenerator(int _x, int _y)
{
	MapGen->LoadChunk(_x, _y);
	ObMgr->SetChunkData(MapGen->GetStreamChunk());

	MapGen->LoadCloudChunk(_x, _y);//구름 로드
	ObMgr->SetCloudChunkData(MapGen->GetStreamCloudChunk());//구름 셋 청크 데이터

	MapGen->LoadObjectChunk(_x, _y); //오브젝트 로드
	ObMgr->SetObjectChunkData(MapGen->GetStreamObjectChunk());//오브젝트 셋 청크 데이터

	LoadItemChunk(_x, _y); //필드 아이템 로드
	ItmMgr->SetItemChunkData(GetStreamItemChunk());//아이템 셋 청크 데이터

	MapGen->LoadAnimalList(_x, _y); //필드 동물 로드
	AnimalMgr->SetAnimalChunkData(MapGen->GetStreamFieldAnimalList()); //동물 셋 청크 데이터
	SetAnimalChunk(MapGen->GetStreamFieldAnimalList());

	CX = _x;
	CY = _y;
}

void AInGameLevel::DisPoseTile(int _x, int _y)
{
	//플레이어가 스폰 후 실행되는 함수
	//아이템은 이미 실행
	ObMgr->DisposeTile(_x, _y);
	StreamAnimalArrayHide();
}

void AInGameLevel::LR_StreamDisposeTile(int _value, int _y, int _Tile_y)
{
	//타일단위로 이동시에만 이동
	ObMgr->LR_StreamDisposeTile(_value, _y, _Tile_y);
	PTY = _y;

	StreamItemArrayHide();
	StreamAnimalArrayHide();
}

void AInGameLevel::FB_StreamDisposeTile(int _value, int _x, int _Tile_x)
{
	//타일단위로 이동시에만 이동
	ObMgr->FB_StreamDisposeTile(_value, _x, _Tile_x);
	PTX = _x;

	StreamItemArrayHide();
	StreamAnimalArrayHide();
}

int AInGameLevel::GetPX()
{
	return PX;
}

int AInGameLevel::GetPY()
{
	return PY;
}

int AInGameLevel::GetPTX()
{
	return PTX;
}

int AInGameLevel::GetPTY()
{
	return PTY;
}

void AInGameLevel::CreateCloudChunk(int _x, int _y, int _sx, int _sy)
{
	MapGen->CreateCloudChunk(_x, _y, _sx, _sy);

	if (CheckInStreamChunk(_x + (_sx * 0.5), _y + (_sy * 0.5)) ||
		CheckInStreamChunk(_x - (_sx * 0.5), _y - (_sy * 0.5)) ||
		CheckInStreamChunk(_x + (_sx * 0.5), _y - (_sy * 0.5)) ||
		CheckInStreamChunk(_x - (_sx * 0.5), _y + (_sy * 0.5)))
	{
		MapGen->LoadCloudChunk(CX, CY);
		ObMgr->ApplyStreamCloudData(MapGen->GetStreamCloudChunk());
	}
	else
	{
		ObMgr->SetCloudChunkData(MapGen->GetStreamCloudChunk());
	}
}

void AInGameLevel::DeleteCloudChunk(int _x, int _y, int _sx, int _sy)
{
	MapGen->DeleteCloudChunk(_x, _y, _sx, _sy);

	if (CheckInStreamChunk(_x + (_sx * 0.5), _y + (_sy * 0.5)) ||
		CheckInStreamChunk(_x - (_sx * 0.5), _y - (_sy * 0.5)) ||
		CheckInStreamChunk(_x + (_sx * 0.5), _y - (_sy * 0.5)) ||
		CheckInStreamChunk(_x - (_sx * 0.5), _y + (_sy * 0.5)))
	{
		MapGen->LoadCloudChunk(CX, CY);
		ObMgr->ApplyStreamCloudData(MapGen->GetStreamCloudChunk());
	}

	else
	{
		ObMgr->SetCloudChunkData(MapGen->GetStreamCloudChunk());
	}
}

void AInGameLevel::DisposeObjChunk(int _x, int _y, GObjectData _data)
{
	_data.Time = PlayTime;

	MapGen->DisposeObjectChunk(_x, _y, _data);

	MapGen->LoadObjectChunk(CX, CY);
	ObMgr->SetObjectChunkData(MapGen->GetStreamObjectChunk());

	if (CheckInStreamChunk(_x, _y))
	{
		ObMgr->ApplyStreamObjData(_x, _y);
	}
}

void AInGameLevel::DeleteObjChunk(int _x, int _y)
{
	//아이템 데이터 생성
	//아이템 수량 설정.
	GObjectData * Data = MapGen->GetObjectData(_x, _y);
	if (Data != nullptr)
	{
		GObjDataBase ObjData = MyGameInstance->ObjDataBase[Data->Code][Data->AidCode];
		TArray<GItemData> ItemData;

		ObjDataBaseToItemData(ObjData, &ItemData);

		for (auto It = ItemData.CreateConstIterator(); It; It++)
		{
			GItemData ITD = ItemData[It.GetIndex()];

			UE_LOG(LogTemp, Warning, TEXT("TTE %d %d %d"), It.GetIndex(), ITD.ItemCode, ITD.ItemAidCode);

			ITD.x = _x;
			ITD.y = _y;
			//ITD.Amount = (rand() % (ObjData.Amount)) + 1;

			int ChunkX = _x / 256;
			int ChunkY = _y / 256;
			int TileX = _x % 256;
			int TileY = _y % 256;

			int UD = ((_x % 256) / 128) * 2;
			int LR = (_y % 256) / 128;
			int UDLR = UD + LR;

			GItemDataNode * Node = ItemDataListAdd(ItemChunkData[ChunkX][ChunkY].Lo[UDLR], ITD);

			MapGen->DeleteObjectChunk(_x, _y);

			MapGen->LoadObjectChunk(CX, CY);
			ObMgr->SetObjectChunkData(MapGen->GetStreamObjectChunk());

			if (CheckInStreamChunk(_x, _y))
			{
				ObMgr->ApplyStreamObjData(_x, _y);
				//아이템 실시간 스폰
				LoadItemChunk(_x, _y);
				ItmMgr->SetItemChunkData(GetStreamItemChunk());
				ItmMgr->ArrayItemActor(_x, _y, CX, CY, ITD, Node);
			}
		}
	}
}

void AInGameLevel::ChangeObjChunk(int _x, int _y, GObjectData _data)
{
	_data.Time = PlayTime;

	MapGen->ChangeObjectData(_x, _y, _data);

	MapGen->LoadObjectChunk(CX, CY);
	ObMgr->SetObjectChunkData(MapGen->GetStreamObjectChunk());

	if (CheckInStreamChunk(_x, _y))
	{
		ObMgr->ApplyStreamObjData(_x, _y);
	}
}

bool AInGameLevel::CheckObjChunk(int _x, int _y)
{
	return MapGen->CheckObjChunk(_x, _y);
}

void AInGameLevel::WeatherToObject(GWeatherData _data, float _DeltaSecond)
{
	MapGen->WeatherToObject(_data, _DeltaSecond);
}

GObjectData * AInGameLevel::GetObjectHead()
{
	return MapGen->GetObjectHead();
}

void AInGameLevel::ObjDataBaseToItemData(GObjDataBase _ObjData, TArray<GItemData> * _ItemDataList)
{
	for (auto It = _ObjData.DestItemCode.CreateConstIterator(); It; It++)
	{
		GItemData ITD;

		ITD.ItemCode = (*It).ItemCode;
		ITD.ItemAidCode = (*It).ItemAidCode;
		ITD.Amount = (rand() % (*It).Amount) + 1;
		ITD.Attack = MyGameInstance->ObjDataBase[ITD.ItemCode][ITD.ItemAidCode].Attack;
		ITD.Sheild = MyGameInstance->ObjDataBase[ITD.ItemCode][ITD.ItemAidCode].Sheild;
		ITD.Distance = MyGameInstance->ObjDataBase[ITD.ItemCode][ITD.ItemAidCode].Distance;

		_ItemDataList->Add(ITD);
	}

	int r = (rand() % 100);
	if (r > 95 && _ObjData.PriceDestItemCode.Num() > 0)
	{
		int ran = rand() % _ObjData.Amount;

		for (int i = 0; i < ran; i++)
		{ 
			int ra = (rand() % _ObjData.PriceDestItemCode.Num());
			FItemCode IC = _ObjData.PriceDestItemCode.Last(ra);
			GItemData ITD;

			ITD.ItemCode = IC.ItemCode;
			ITD.ItemAidCode = IC.ItemAidCode;
			ITD.Amount = (rand() % IC.Amount) + 1;
			ITD.Attack = MyGameInstance->ObjDataBase[ITD.ItemCode][ITD.ItemAidCode].Attack;
			ITD.Sheild = MyGameInstance->ObjDataBase[ITD.ItemCode][ITD.ItemAidCode].Sheild;
			ITD.Distance = MyGameInstance->ObjDataBase[ITD.ItemCode][ITD.ItemAidCode].Distance;

			_ItemDataList->Add(ITD);
		}
	}
}

void AInGameLevel::Test()
{
	UE_LOG(LogTemp, Warning, TEXT("Test"));
}

void AInGameLevel::BeginPlay()
{
	Super::BeginPlay();

	if (MapGen != nullptr && ObMgr != nullptr  && WeatherSys != nullptr)
	{
		delete(MapGen);
		delete(ObMgr);
		delete(WeatherSys);
	}

	MyGameInstance = Cast<UMyGameInstance>(GetGameInstance());

	MapGen = new MapGenerator(this);
	ObMgr = new ObjectMgr(this);
	WeatherSys = new WeatherSystem(this);
	ItmMgr = new ItemMgr(this);
	AnimalMgr = new AniamlMgr(this);

	MapSeed = rand();

	for (int x = 0; x < 32; x++)
	{
		for (int y = 0; y < 32; y++)
		{
			ItemChunkData[x][y].Lo[0] = new GItemDataList;
			ItemDataListInit(ItemChunkData[x][y].Lo[0]);

			ItemChunkData[x][y].Lo[1] = new GItemDataList;
			ItemDataListInit(ItemChunkData[x][y].Lo[1]);
			
			ItemChunkData[x][y].Lo[2] = new GItemDataList;
			ItemDataListInit(ItemChunkData[x][y].Lo[2]);

			ItemChunkData[x][y].Lo[3] = new GItemDataList;
			ItemDataListInit(ItemChunkData[x][y].Lo[3]);

			ItemChunkData[x][y].x = x;
			ItemChunkData[x][y].y = y;
		}
	}

	MapGen->GenerateMap(MapSeed);
	MapGen->CreateAnimal();
	UE_LOG(LogTemp, Warning, TEXT("Seed %d"), MapSeed);

	PX = MapGen->GetPX();
	PY = MapGen->GetPY();

	CX = PX / 256;
	CY = PY / 256;

	PTX = PX * 0.01;
	PTY = PY * 0.01;

	SetActorTickEnabled(true);

	ObMgr->SpawnTileActor(GetWorld()); //타일 스폰
	ItmMgr->SpawnItemActor(GetWorld()); //아이템 스폰
	AnimalMgr->SpawnAnimalActor(GetWorld()); //동물 스폰

	bCanSpawnPlayer = true;
}

void AInGameLevel::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	PlayTime += DeltaSeconds;

	WeatherSys->Tick(DeltaSeconds);
}

bool AInGameLevel::CheckInStreamChunk(int _x, int _y)
{
	int MinX = PTX - ADJUST_X_M;
	int MaxX = PTX + ADJUST_X_P;
	int MinY = PTY - ADJUST_Y_M;
	int MaxY = PTY + ADJUST_Y_P;

	if (MinX <= 0)
		MinX = 0;
	else if (MaxX > 8192)
		MaxX = 8192;

	if (MinY <= 0)
		MinY = 0;
	else if (MaxY >= 8192)
		MaxY = 8192;

	if (_x >= MinX && _x <= MaxX && _y >= MinY && _y <= MaxY)
		return true;

	return false;
}

bool AInGameLevel::CheckInItemStreamChunk(int _x, int _y)
{
	int DistanceX = PTX - _x;
	int DistanceY = PTY - _y;

	if (-64 - ADJUST_X_M < DistanceX && DistanceX < 64 + ADJUST_X_P && -64 - ADJUST_Y_M < DistanceY && DistanceY < 64 + ADJUST_Y_P)
	{
		return true;
	}

	return false;
}

bool AInGameLevel::CheckInAnimalStreamChunk(int _x, int _y)
{
	int DistanceX = PTX - _x;
	int DistanceY = PTY - _y;

	if (-64 - ADJUST_X_M < DistanceX && DistanceX < 64 + ADJUST_X_P && -64 - ADJUST_Y_M < DistanceY && DistanceY < 64 + ADJUST_Y_P)
	{
		return true;
	}

	return false;
}

void AInGameLevel::LoadItemChunk(int _x, int _y)
{
	for (int col = _x - 1; col <= _x + 1; col++)
	{
		for (int row = _y - 1; row <= _y + 1; row++)
		{
			if (col >= 0 && row >= 0 && col < 32 && row < 32)
			{
				StreamItemChunkData[col - _x + 1][row - _y + 1] = ItemChunkData[col][row];
			}
		}
	}
}

void AInGameLevel::ItemStreamChunkToItemChunk()
{
	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			int ChunkX = StreamItemChunkData[x][y].x;
			int ChunkY = StreamItemChunkData[x][y].y;
			ItemChunkData[ChunkX][ChunkY] = StreamItemChunkData[x][y];
		}
	}
}

void AInGameLevel::StreamItemArrayHide()
{
	for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++)
			for (int i = 0; i < 4; i++)
			{
				int UD = i / 2; //UD 계산
				int LR = i % 2; //LR 계산

				int TileX = (StreamItemChunkData[x][y].x * 256) + (UD * 128) + 64;
				int TileY = (StreamItemChunkData[x][y].y * 256) + (LR * 128) + 64;

				//실시간 범위에 대한 계산
				if (CheckInItemStreamChunk(TileX, TileY))
				{
					if (!StreamItemChunkData[x][y].bLo[i])
					{
						//아이템을 배치하는 코드
						ItmMgr->ArrayItemChunkActor(x, y, i);
					}
					//UE_LOG(LogTemp, Warning, TEXT("StreamItem %d %d %d"), x, y, i);
					StreamItemChunkData[x][y].bLo[i] = true;
				}

				else
				{
					if (StreamItemChunkData[x][y].bLo[i])
					{
						ItmMgr->HideItemChunkActor(x, y, i);
						StreamItemChunkData[x][y].bLo[i] = false;
					}
				}
			}

	ItemStreamChunkToItemChunk();
}

void AInGameLevel::ItemSpawn(int _x, int _y, GItemDataNode * _node)
{
	int ChunkX = _x / 256;
	int ChunkY = _y / 256;
	int TileX = _x % 256;
	int TileY = _y % 256;

	int UD = ((_x % 256) / 128) * 2;
	int LR = (_y % 256) / 128;
	int UDLR = UD + LR;

	ItemDataListAdd(ItemChunkData[ChunkX][ChunkY].Lo[UDLR], _node);

	if (CheckInStreamChunk(_x, _y))
	{
		ObMgr->ApplyStreamObjData(_x, _y);
		//아이템 실시간 스폰
		LoadItemChunk(_x, _y);
		ItmMgr->SetItemChunkData(GetStreamItemChunk());
		ItmMgr->ArrayItemActor(_x, _y, CX, CY, _node->Data, _node);
	}
}

void AInGameLevel::LootingItem(int _x, int _y, int _num, GItemDataNode * _node, APawn * _pawn)
{
	//플레이어 인벤토리에 추가하는 코드
	int Result = Cast<APlayerPawn>(_pawn)->LootingItem(_node);
	if (Result != 0)
	{
		int ChunkX = _x / 256;
		int ChunkY = _y / 256;

		int UD = ((_x % 256) / 128) * 2;
		int LR = (_y % 256) / 128;
		int UDLR = UD + LR;

		ItmMgr->LootingItem(ChunkX, ChunkY, UDLR, _num);
		RemoveNode(ItemChunkData[ChunkX][ChunkY].Lo[UDLR], _node);
		LoadItemChunk(ChunkX, ChunkY);
		ItmMgr->SetItemChunkData(GetStreamItemChunk());

		UE_LOG(LogTemp, Warning, TEXT("LootingItem"));
	}

	if (Result == 2)
		delete(_node);
}

void AInGameLevel::TempItemPlace(int _x, int _y, GItemData _data)
{
	ObMgr->TempItemPlace(_x, _y, _data);
}

void AInGameLevel::HideTempItemPlace()
{
	ObMgr->HideTempItemPlace();
}

void AInGameLevel::StreamAnimalArrayHide()
{
	for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++)
			for (int i = 0; i < 4; i++)
			{
				int UD = i / 2; //UD 계산
				int LR = i % 2; //LR 계산

				int TileX = (StreamAnimalChunkData[x][y].x * 256) + (UD * 128) + 64;
				int TileY = (StreamAnimalChunkData[x][y].y * 256) + (LR * 128) + 64;

				//실시간 범위에 대한 계산
				if (CheckInAnimalStreamChunk(TileX, TileY))
				{
					if (!StreamAnimalChunkData[x][y].bLo[i])
					{
						//아이템을 배치하는 코드
						AnimalMgr->ArrayAnimalChunkActor(x, y, i);
					}
					//UE_LOG(LogTemp, Warning, TEXT("StreamItem %d %d %d"), x, y, i);
					StreamAnimalChunkData[x][y].bLo[i] = true;
				}

				else
				{
					if (StreamAnimalChunkData[x][y].bLo[i])
					{
						AnimalMgr->HideAnimalChunkActor(x, y, i);
						StreamAnimalChunkData[x][y].bLo[i] = false;
					}
				}
			}

	MapGen->SetAnimalChunk(StreamAnimalChunkData);
}

void AInGameLevel::SetAnimalChunk(GFieldAnimalList(*_AnimalData)[3])
{
	StreamAnimalChunkData = _AnimalData;
}

GFieldItemList(*AInGameLevel::GetStreamItemChunk())[3]
{
	return StreamItemChunkData;
}
