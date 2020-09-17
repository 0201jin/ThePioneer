// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Object/ObjectDataClass.h"
#include "Object/ObjectClass.h"

#include "CoreMinimal.h"

/**
 * 
 */
class THEPIONEER_API Furnace : public ObjectDataClass
{
public:
	Furnace();
	~Furnace();

	void Tick(float DeltaTime, int x, int y);
	void SetInGameLevel(AInGameLevel * _InGameLevel);

	void SetParent(AActor * _parent);
	void Action(APlayerController * _Controller);

	void SetData(GObjectData * _Data);
	GObjectData * GetData();

	void SetMaterialItem(GItemDataNode * _Material, int _Num);
	void SetResultItem(GItemDataNode * _Result);

	GItemDataNode * GetMaterialItem(int _Num);
	GItemDataNode * GetResultItem();

	bool GetActive();

	void Damage(int _Damage);
	void InitHealth();

	void StartCrafting();

protected:
	AObjectClass * Parent;
	AInGameLevel * InGameLevel;

	GItemData CraftItemData;

	GObjectData * Data;

	TArray<FItemMaterial> Item_Material_Array;
	GItemDataNode * Material_Item[3] = { nullptr, nullptr, nullptr };
	GItemDataNode * Result_Item = nullptr;

	bool bFindCraftItem = false;
	bool bCanCraft = false;

	int Health = 100;

	float Crafting_Time = 0; //줄어드는 시간
	float Crafting_Time_All = 0;
	float Crafting_Time_One = 0;
};
