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

	void Tick(float DeltaSeconds); //Weather 실시간으로 날씨를 관리하는 코드 작성

protected:
	void NewWeather(); //월드에 새로운 날씨 이벤트 생성

protected:
	GWeatherList * WeatherList;
	AInGameLevel * InGameLevel;
};
