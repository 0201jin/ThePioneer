// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"
#include "Engine.h"
#include "Object/ObjectClass.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); //값을 올리면 옆으로 흐르듯 이동
	GetCharacterMovement()->JumpZVelocity = 0.0f;
	GetCharacterMovement()->MaxStepHeight = 0.0f;
	GetCharacterMovement()->MaxWalkSpeed = 550.0f;
	GetCharacterMovement()->AirControl = 0.0f;
	GetCharacterMovement()->GroundFriction = 100.0f;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Mesh1(TEXT("SkeletalMesh'/Game/Modeling/Character/Character.Character'"));

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UClass> AnimClass(TEXT("AnimBlueprint'/Game/Modeling/Character/Animation/Character_Animation_Blueprint.Character_Animation_Blueprint_C'"));
	SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SkeletalMesh->AnimClass = AnimClass.Object;
	SkeletalMesh->SetSkeletalMesh(Mesh1.Object);
	SkeletalMesh->SetRelativeLocation(FVector(0, 0, -70));
	SkeletalMesh->SetRelativeScale3D(FVector(30, 30, 30));
	SkeletalMesh->SetRelativeRotation(FRotator(0, -90, 0));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 700.0f;
	CameraBoom->TargetOffset = FVector(0, 0, 900);
	CameraBoom->ProbeSize = 0.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->SetRelativeLocation(FVector(0, 0, 0));
	FollowCamera->SetRelativeRotation(FRotator(307, 0, 0));

	CameraCollision = NewObject<UBoxComponent>(this, TEXT("CollisionBox"));
	CameraCollision->SetupAttachment(FollowCamera);
	CameraCollision->SetRelativeLocation(FVector(550, 0, 0));
	CameraCollision->InitBoxExtent(FVector(550, 6, 6));
	CameraCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerPawn::OnOverlapBegin);
	CameraCollision->OnComponentEndOverlap.AddDynamic(this, &APlayerPawn::OnOverlapEnd);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DigMo(TEXT("AnimMontage'/Game/Modeling/Character/Animation/Character_Dig_Montage.Character_Dig_Montage'"));
	DigAnimation = DigMo.Object;
}

void APlayerPawn::FB_Move(float _value)
{
	if (_value)
	{
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, _value);

		if (ChunkX != (int)(GetActorLocation().X / (256 * 100)))
		{
			ChunkX = GetActorLocation().X / (256 * 100);
			LevelScript->LoadChunkToMapGenerator(ChunkX, ChunkY);
		}

		int TileLo = (GetActorLocation().X * 0.01 + 0.5) - (ChunkX * 256);
		int Lo = GetActorLocation().X * 0.01 + 0.5;

		if (Lo != LoX)
		{
			if (Lo < LoX) //아래 이동
			{
				int Tile_X = GetActorLocation().X * 0.01 + 0.5;

				LevelScript->FB_StreamDisposeTile(-1, Tile_X, TileLo);
				LoX = Lo;
			}

			else if (Lo > LoX) //위 이동
			{
				int Tile_X = GetActorLocation().X * 0.01 + 0.5;

				LevelScript->FB_StreamDisposeTile(1, Tile_X, TileLo);
				LoX = Lo;
			}
		}
	}
}

void APlayerPawn::LR_Move(float _value)
{
	if (_value)
	{
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, _value);

		if (ChunkY != (int)(GetActorLocation().Y / (256 * 100)))
		{
			ChunkY = GetActorLocation().Y / (256 * 100);
			LevelScript->LoadChunkToMapGenerator(ChunkX, ChunkY);
		}

		int TileLo = (GetActorLocation().Y * 0.01) - (ChunkY * 256);
		int Lo = GetActorLocation().Y * 0.01 + 0.5;

		if (Lo != LoY)
		{
			if (Lo < LoY) //왼쪽 이동
			{
				int Tile_Y = GetActorLocation().Y * 0.01 + 0.5;

				LevelScript->LR_StreamDisposeTile(-1, Tile_Y, TileLo);
				LoY = Lo;
			}

			else if (Lo > LoY) //오른쪽 이동
			{
				int Tile_Y = GetActorLocation().Y * 0.01 + 0.5;

				LevelScript->LR_StreamDisposeTile(1, Tile_Y, TileLo);
				LoY = Lo;
			}
		}
	}
}

void APlayerPawn::PosDisPoseTile()
{
	LevelScript->DisPoseTile((GetActorLocation().X * 0.01), (GetActorLocation().Y * 0.01));
}

void APlayerPawn::ObjectDisPose()
{
	GObjectData _data;
	_data.Code = 0;
	_data.AidCode = 4;

	LevelScript->DisposeObjChunk(GetActorLocation().X * 0.01, GetActorLocation().Y * 0.01, _data);
}

void APlayerPawn::ObjectDig(AActor * _actor)
{
	if (Cast<AObjectClass>(_actor))
	{
		int Set = Attack;

		if(PickItemUse == 1)
			Set *= 2;

		int Damage = Attack * Set;
		Cast<AObjectClass>(_actor)->Damage(Attack);
		SkeletalMesh->GetAnimInstance()->Montage_Play(DigAnimation);
	}
}

int APlayerPawn::LootingItem(GItemDataNode * _node)
{
	if (_node)
	{
		int Max_Amount = MyGameInstance->ObjDataBase[_node->Data.ItemCode][_node->Data.ItemAidCode].Max_Amount;

		if (!MyGameInstance->ObjDataBase[_node->Data.ItemCode][_node->Data.ItemAidCode].Can_Wear)
		{
			for (int i = 0; i < INVEN_SIZE; i++) //겹치는 코드
			{
				if ((*(*Inven + i)) != nullptr)
				{
					if (((*(*Inven + i))->Data.ItemCode != 0 && (*(*Inven + i))->Data.ItemAidCode != 0))
					{
						GItemData InvenData = (*(*Inven + i))->Data;
						GItemData NodeData = _node->Data;

						int Amount = (*(*Inven + i))->Data.Amount + NodeData.Amount;

						if (InvenData.ItemCode == NodeData.ItemCode &&
							InvenData.ItemAidCode == NodeData.ItemAidCode &&
							Amount <= Max_Amount)
						{
							(*(*Inven + i))->Data.Amount += NodeData.Amount;
							_node->Data.Amount -= NodeData.Amount;
							MyHUD->SetInven(GetInven());
							PickUseInven(UseInven_Index);
							return 2;
						}

						if (InvenData.ItemCode == NodeData.ItemCode &&
							InvenData.ItemAidCode == NodeData.ItemAidCode &&
							(*(*Inven + i))->Data.Amount < Max_Amount &&
							_node->Data.Amount > 0)
						{
							int R = Max_Amount - (*(*Inven + i))->Data.Amount;
							(*(*Inven + i))->Data.Amount += R;
							_node->Data.Amount -= R;
							i = 0;
						}
					}
				}
			}
		}

		for (int a = 0; a < INVEN_SIZE; a++) //안겹치는 코드
		{
			if ((*(*Inven + a)) == nullptr)
			{
				(*(*Inven + a)) = _node;
				MyHUD->SetInven(GetInven());
				PickUseInven(UseInven_Index);
				return 1;
			}

			else if (((*(*Inven + a))->Data.ItemCode == 0 && (*(*Inven + a))->Data.ItemAidCode == 0))
			{
				(*(*Inven + a)) = _node;
				MyHUD->SetInven(GetInven());
				PickUseInven(UseInven_Index);
				return 1;
			}
		}
	}

	return 0;
}

GItemDataNode *(*APlayerPawn::GetInven())[INVEN_ROW]
{
	return Inven;
}

void APlayerPawn::SetHUD(AMyHUD * _MyHUD)
{
	MyHUD = _MyHUD;
}

void APlayerPawn::SwapInven(int _Num, int _NewNum)
{
	if (_Num != _NewNum)
	{
		GItemDataNode * Save;
		GItemDataNode ** Num;
		GItemDataNode ** NewNum;

		bool bCheckStat = false;

		if (_Num >= 30)
		{
			Num = &(*(Armour + (_Num % 30)));
			bCheckStat = true;
		}

		else
			Num = &(*(*Inven + _Num));

		if (_NewNum >= 30)
		{
			int Number = _NewNum % 30;
			NewNum = &(*(Armour + Number));
			bCheckStat = true;
			if (!MyGameInstance->ObjDataBase[(*Num)->Data.ItemCode][(*Num)->Data.ItemAidCode].Can_Wear)
				return;

			if (Number != (MyGameInstance->ObjDataBase[(*Num)->Data.ItemCode][(*Num)->Data.ItemAidCode].Parts - 2))
				return;
		}

		else
			NewNum = &(*(*Inven + _NewNum));

		Save = *NewNum;

		if ((*NewNum) != nullptr)
		{
			if ((*Num)->Data.ItemCode == (*NewNum)->Data.ItemCode &&
				(*Num)->Data.ItemAidCode == (*NewNum)->Data.ItemAidCode)
			{
				(*(*Inven + _NewNum))->Data.Amount += (*(*Inven + _Num))->Data.Amount;
				delete((*(*Inven + _Num)));
			}
			else
			{
				*NewNum = *Num;
				*Num = Save;
			}
		}
		else
		{
			*NewNum = *Num;
			*Num = Save;
		}

		if (bCheckStat)
			CheckArmourStat();

		PickUseInven(UseInven_Index);

		UE_LOG(LogTemp, Warning, TEXT("SWAPINVEN"));
	}
}

void APlayerPawn::SetInven(GItemDataNode *(*_Inven)[INVEN_ROW])
{
	for (int i = 0; i < INVEN_SIZE; i++)
		*(*Inven + i) = *(*_Inven + i);
}

void APlayerPawn::PickUseInven(int _Num)
{
	UseInven_Index = _Num;

	Attack = 5;
	Distance = 1;

	if (Inven[4][_Num])
	{
		GItemData Data = Inven[4][_Num]->Data;

		if (MyGameInstance->ObjDataBase[Data.ItemCode][Data.ItemAidCode].Can_Wear)
		{
			PickItemUse = MyGameInstance->ObjDataBase[Data.ItemCode][Data.ItemAidCode].Used;
			Attack += Inven[4][_Num]->Data.Attack;
			Distance = Inven[4][_Num]->Data.Distance;
		}

		else if (MyGameInstance->ObjDataBase[Data.ItemCode][Data.ItemAidCode].Can_Dispose)
		{
			//아이템을 설치하는 코드
			bPlaceItem = true;
			PlaceItemData = Data;
			//설치하는 아이템의 인벤 주소 저장
			PlaceInvenAdr = Inven[4][_Num];
			return;
		}
	}
}

void APlayerPawn::UseItem(int _Num)
{
	if ((*(*Inven + _Num)))
	{
		GItemData Data = (*(*Inven + _Num))->Data;

		if (MyGameInstance->ObjDataBase[Data.ItemCode][Data.ItemAidCode].Can_Wear)
		{
			switch (MyGameInstance->ObjDataBase[Data.ItemCode][Data.ItemAidCode].Parts)
			{
			case 2:
				SwapInven(_Num, 30);
				break;

			case 3:
				SwapInven(_Num, 31);
				break;

			case 4:
				SwapInven(_Num, 32);
				break;
			}
			return;
		}

		else if (MyGameInstance->ObjDataBase[Data.ItemCode][Data.ItemAidCode].Used)
		{
			//아이템을 사용하는 코드 
			return;
		}

		else if (MyGameInstance->ObjDataBase[Data.ItemCode][Data.ItemAidCode].Can_Dispose)
		{
			//아이템을 설치하는 코드
			bPlaceItem = true;
			PlaceItemData = Data;
			//설치하는 아이템의 인벤 주소 저장
			PlaceInvenAdr = (*(*Inven + _Num));
			return;
		}
	}
}

void APlayerPawn::CameraRotation_L()
{
	AddControllerYawInput(36);
}

void APlayerPawn::CameraRotation_R()
{
	AddControllerYawInput(-36);
}

int APlayerPawn::GetDistance()
{
	return Distance * 100 + 50;
}

void APlayerPawn::ItemPlaceMode()
{
	//아이템 배치 코드
	FHitResult Hit;
	Cast<APlayerController>(Controller)->GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		FVector Lo = Hit.GetActor()->GetActorLocation();
		CurX = Lo.X;
		CurY = Lo.Y;

		LevelScript->TempItemPlace(CurX, CurY, PlaceItemData);
	}
}

void APlayerPawn::ItemPlace()
{
	bPlaceItem = false;

	GObjDataBase ODB = MyGameInstance->ObjDataBase[PlaceItemData.ItemCode][PlaceItemData.ItemAidCode];

	GObjectData data;
	data.Code = ODB.DisposeItemCode;
	data.AidCode = ODB.DisposeItemAidCode;
	data.Grow = 0;
	data.Exp = 0;
	data.FExp = 0;
	data.Time = 0;

	if (LevelScript->CheckObjChunk(CurX * 0.01, CurY * 0.01)
		&& !(LevelScript->GetPTX() == CurX * 0.01 && LevelScript->GetPTY() == CurY * 0.01))
	{
		LevelScript->HideTempItemPlace();

		LevelScript->DisposeObjChunk(CurX * 0.01, CurY * 0.01, data);
		//아이템 수 줄이는 코드
		//아이템의 저장된 주소값을 이용해서 수량 줄임
		int Amount = PlaceInvenAdr->Data.Amount - 1;

		if (Amount <= 0)
			memset(PlaceInvenAdr, 0, sizeof(PlaceInvenAdr));

		else
			PlaceInvenAdr->Data.Amount -= 1;

		MyHUD->SetInven(Inven);
	}
}

bool APlayerPawn::GetPlaceMode()
{
	return bPlaceItem;
}

GItemDataNode * APlayerPawn::GetHead()
{
	return Armour[0];
}

GItemDataNode * APlayerPawn::GetBody()
{
	return Armour[1];
}

GItemDataNode * APlayerPawn::GetShoose()
{
	return Armour[2];
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	LevelScript = Cast<AInGameLevel>(GetWorld()->GetLevelScriptActor());
	MyGameInstance = LevelScript->MyGameInstance;

	Anim = Cast<UPlayerAnimation>(SkeletalMesh->GetAnimInstance());
	//여기에 플레이어 위치 이동 코드(레벨에서 플레이어 스폰위치 가져오기)
}

void APlayerPawn::CheckArmourStat()
{
	int PHealth = 0;
	int PSheild = 0;

	Sheild = 0;
	Max_Health = 100;

	for (int i = 0; i < 3; i++)
	{
		if (Armour[i] != nullptr)
			if (Armour[i]->Data.ItemCode != 0 && Armour[i]->Data.ItemAidCode != 0)
				PHealth += Armour[i]->Data.Health;
	}

	for (int i = 0; i < 3; i++)
	{
		if (Armour[i] != nullptr)
			if (Armour[i]->Data.ItemCode != 0 && Armour[i]->Data.ItemAidCode != 0)
				PSheild += Armour[i]->Data.Sheild;
	}

	Sheild += PSheild;
	Max_Health += PHealth;

	UE_LOG(LogTemp, Warning, TEXT("%d %d"), Sheild, Max_Health);
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LevelScript->GetPlayerSpawn() && CountA == 0)
	{
		SetActorLocation(FVector(LevelScript->GetPX(), LevelScript->GetPY(), 422));

		ChunkX = (LevelScript->GetPX() * 0.01) / (256);
		ChunkY = (LevelScript->GetPY() * 0.01) / (256);
		LevelScript->LoadChunkToMapGenerator(ChunkX, ChunkY);
		PosDisPoseTile();
		LevelScript->StreamItemArrayHide();
		CountA++;
	}

	//Health--;

	if (Health == 0)
		Anim->CharacterDead(true);

	if (bPlaceItem)
		ItemPlaceMode();
}


void APlayerPawn::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<AObjectClass>(OtherActor))
		Cast<AObjectClass>(OtherActor)->FadeIn();
}

void APlayerPawn::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (Cast<AObjectClass>(OtherActor))
		Cast<AObjectClass>(OtherActor)->FadeOut();
}