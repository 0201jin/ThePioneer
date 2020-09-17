// Fill out your copyright notice in the Description page of Project Settings.

#include "WeatherSystem.h"

using namespace MapStruct;

WeatherSystem::WeatherSystem(AInGameLevel * _InGameLevel)
{
	WeatherList = new GWeatherList;
	UE_LOG(LogTemp, Warning, TEXT("WeatherSystem On"));
	WeatherListInit(WeatherList);

	UE_LOG(LogTemp, Warning, TEXT("%d"), WeatherList->NumOfData);

	InGameLevel = _InGameLevel;
}

WeatherSystem::~WeatherSystem()
{
}

void WeatherSystem::Tick(float DeltaSeconds)
{
	//������ �����ϴ� �ڵ� ��������
	int MaxProbability = (20 - (WeatherList->NumOfData));
	if (MaxProbability > 0)
	{
		NewWeather();
	}

	//������ ó���ϴ� �ڵ�
	GWeatherData _Data;
	WeatherListFirst(WeatherList, &_Data);
	GetWeatherNode(WeatherList)->Data.time -= DeltaSeconds;
	InGameLevel->WeatherToObject(_Data, DeltaSeconds);

	if (_Data.time <= 0)
	{
		RemoveNode(WeatherList, GetWeatherNode(WeatherList));
		InGameLevel->DeleteCloudChunk(_Data.x, _Data.y, _Data.sx, _Data.sy);
		UE_LOG(LogTemp, Warning, TEXT("Delete Weather Event"));
	}

	for (int i = 0; i < WeatherList->NumOfData - 1; i++)
	{
		GWeatherData Data;

		WeatherListNext(WeatherList, &Data);
		GetWeatherNode(WeatherList)->Data.time -= DeltaSeconds;
		InGameLevel->WeatherToObject(_Data, DeltaSeconds);

		if (Data.time <= 0)
		{
			RemoveNode(WeatherList, GetWeatherNode(WeatherList));
			InGameLevel->DeleteCloudChunk(Data.x, Data.y, Data.sx, Data.sy);
			UE_LOG(LogTemp, Warning, TEXT("Delete Weather Event"));
		}
	}
}

void WeatherSystem::NewWeather()
{
	GWeatherData Data;

	Data.Rain = 1;

	Data.x = (rand() % 8192); //��ġ 8192 0
	Data.y = (rand() % 8192); //��ġ 8192 0
	
	Data.sx = (rand() % 200) + 50; //���� 200 50
	Data.sy = (rand() % 200) + 50; //���� 200 50

	Data.time = (rand() % 540) + 60; //�ð� 540 60

	WeatherListAdd(WeatherList, Data);

	InGameLevel->CreateCloudChunk(Data.x, Data.y, Data.sx, Data.sy);

	UE_LOG(LogTemp, Warning, TEXT("Create Weather Event"));
}
