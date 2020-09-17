// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "System/MapStruct.h"
#include "LevelScript/InGameLevel.h"

#include "CoreMinimal.h"

using namespace MapStruct;
/**
 * 
 */

class THEPIONEER_API WeatherSystem
{
public:
	WeatherSystem(AInGameLevel * _InGameLevel);
	~WeatherSystem();

	void Tick(float DeltaSeconds); //Weather �ǽð����� ������ �����ϴ� �ڵ� �ۼ�

protected:
	void NewWeather(); //���忡 ���ο� ���� �̺�Ʈ ����

protected:
	GWeatherList * WeatherList;
	AInGameLevel * InGameLevel;
};
