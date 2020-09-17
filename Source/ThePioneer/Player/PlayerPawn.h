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

	void FB_Move(float _value); //���� �̵�
	void LR_Move(float _value); //�¿� �̵�

	void PosDisPoseTile(); //Ÿ�� �ʱ��ġ ��û

	void ObjectDisPose(); //������Ʈ ��ġ(��ġ)

	void ObjectDig(AActor * _actor); //������Ʈ �ı�

	int LootingItem(GItemDataNode * _node); //������ ����

	GItemDataNode *(*GetInven())[INVEN_ROW]; //�κ� ��ȯ

	void SetHUD(AMyHUD * _MyHUD); //HUD ����

	void SwapInven(int _Num, int _NewNum); //�κ� ����
	void SetInven(GItemDataNode*(*_Inven)[INVEN_ROW]);

	void PickUseInven(int _Num); //������ ����
	void UseItem(int _Num); //������ ���(�κ� ������ ��Ŭ��)
	 
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
