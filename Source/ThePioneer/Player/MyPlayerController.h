// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "PlayerPawn.h"
#include "Object/ObjectClass.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class THEPIONEER_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

	AMyPlayerController();

public:
	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		void SwapInven(int _Num, int _NewNum);

	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		void UseItem(int _Num);

public:
	virtual void SetupInputComponent();
	virtual void PlayerTick(float DeltaTime) override;

public:
	GItemDataNode *(*GetInven())[INVEN_ROW];
	void CraftItem(GItemDataNode * _Item);
	void SetInven(GItemDataNode*(*_Inven)[INVEN_ROW]);

	GItemDataNode * GetHead();
	GItemDataNode * GetBody();
	GItemDataNode * GetShoose();

protected:
	virtual void BeginPlay() override;

protected:
	void FB_Move(float _value);
	void LR_Move(float _value);
	void Farming(float _value);

	void L_Click();
	void R_Click();
	void OpenInven();
	void Use_Inven_Swap();

	void CameraRotation_L();
	void CameraRotation_R();

	void SetDigTimer();
	
protected:
	APlayerPawn * PlayerPawn;
	AMyHUD * MyHUD;

	bool bCanDig = true;

	FTimerHandle DigTimer;
};
