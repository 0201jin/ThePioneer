// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LevelScript/InGameLevel.h"
#include "PlayerAnimation.h"
#include "UI/MyHUD.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerPawn.generated.h"

UCLASS()
class THEPIONEER_API APlayerPawn : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UCameraComponent * FollowCamera;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USpringArmComponent * CameraBoom;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent * SkeletalMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UBoxComponent * CameraCollision;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Sets default values for this character's properties
	APlayerPawn();

	void FB_Move(float _value); //상하 이동
	void LR_Move(float _value); //좌우 이동

	void PosDisPoseTile(); //타일 초기배치 요청

	void ObjectDisPose(); //오브젝트 배치(설치)

	void ObjectDig(AActor * _actor); //오브젝트 파괴

	int LootingItem(GItemDataNode * _node); //아이템 루팅

	GItemDataNode *(*GetInven())[INVEN_ROW]; //인벤 반환

	void SetHUD(AMyHUD * _MyHUD); //HUD 설정

	void SwapInven(int _Num, int _NewNum); //인벤 스왑
	void SetInven(GItemDataNode*(*_Inven)[INVEN_ROW]);

	void PickUseInven(int _Num); //아이템 선택
	void UseItem(int _Num); //아이템 사용(인벤 아이템 우클릭)
	 
	void CameraRotation_L();
	void CameraRotation_R();

	int GetDistance();

	void ItemPlaceMode();
	void ItemPlace();

	bool GetPlaceMode();

	GItemDataNode * GetHead();
	GItemDataNode * GetBody();
	GItemDataNode * GetShoose();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CheckArmourStat();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	AInGameLevel * LevelScript;
	UMyGameInstance * MyGameInstance;
	AMyHUD * MyHUD;

	UPlayerAnimation * Anim;
	GItemDataNode * Inven[INVEN_COL][INVEN_ROW] = { 0 };

	GItemDataNode * Armour[3] = { 0 };

	GItemDataNode * PlaceInvenAdr;
	GItemData PlaceItemData;

	class UAnimMontage * DigAnimation;

	int ChunkX, ChunkY = 0;
	int TileX, TileY = 0;
	int LoX, LoY = 0;
	int CountA = 0;

	int UseInven_Index = 0;

	int Health = 100;
	int Max_Health = 100;
	int Sheild = 0;
	int Attack = 5;
	int Distance = 1;
	int PickItemUse = 0;

	int CurX, CurY = 0;

	bool bPlaceItem = false;
};
