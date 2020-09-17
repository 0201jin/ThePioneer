// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"

AMyPlayerController::AMyPlayerController()
{
	bShowMouseCursor = true;
}

void AMyPlayerController::SwapInven(int _Num, int _NewNum)
{
	PlayerPawn->SwapInven(_Num, _NewNum);
	MyHUD->SetInven(PlayerPawn->GetInven());
}

void AMyPlayerController::UseItem(int _Num)
{
	PlayerPawn->UseItem(_Num);
	MyHUD->SetInven(PlayerPawn->GetInven());
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAction("CameraRotation_L", IE_Pressed, this, &AMyPlayerController::CameraRotation_L);
	InputComponent->BindAction("CameraRotation_R", IE_Pressed, this, &AMyPlayerController::CameraRotation_R);
	InputComponent->BindAction("L_Click", IE_Pressed, this, &AMyPlayerController::L_Click);
	InputComponent->BindAction("R_Click", IE_Pressed, this, &AMyPlayerController::R_Click);
	InputComponent->BindAction("OpenInven", IE_Pressed, this, &AMyPlayerController::OpenInven);
	InputComponent->BindAction("Use_Inven_Swap", IE_Pressed, this, &AMyPlayerController::Use_Inven_Swap);

	InputComponent->BindAxis("FB_Move", this, &AMyPlayerController::FB_Move);
	InputComponent->BindAxis("LR_Move", this, &AMyPlayerController::LR_Move);
	InputComponent->BindAxis("Farming", this, &AMyPlayerController::Farming);
}

void AMyPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

GItemDataNode *(*AMyPlayerController::GetInven())[INVEN_ROW]
{
	return PlayerPawn->GetInven();
}

void AMyPlayerController::CraftItem(GItemDataNode * _Item)
{
	int Result = PlayerPawn->LootingItem(_Item);

	if (Result == 2)
		delete(_Item);
}

void AMyPlayerController::SetInven(GItemDataNode *(*_Inven)[INVEN_ROW])
{
	PlayerPawn->SetInven(_Inven);
}

GItemDataNode * AMyPlayerController::GetHead()
{
	return PlayerPawn->GetHead();
}

GItemDataNode * AMyPlayerController::GetBody()
{
	return PlayerPawn->GetBody();
}

GItemDataNode * AMyPlayerController::GetShoose()
{
	return PlayerPawn->GetShoose();
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = Cast<APlayerPawn>(GetPawn());
	MyHUD = Cast<AMyHUD>(GetHUD());

	PlayerPawn->SetHUD(MyHUD);
}

void AMyPlayerController::FB_Move(float _value)
{
	//UE_LOG(LogTemp, Warning, TEXT("FB_Move"));
	PlayerPawn->FB_Move(_value);
}

void AMyPlayerController::LR_Move(float _value)
{
	//UE_LOG(LogTemp, Warning, TEXT("LR_Move"));
	PlayerPawn->LR_Move(_value);
}

void AMyPlayerController::Farming(float _value)
{
	if(_value)
		L_Click();
}

void AMyPlayerController::L_Click()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (PlayerPawn->GetPlaceMode())
	{
		PlayerPawn->ItemPlace();
	}

	else if (Hit.bBlockingHit)
	{
		if (Cast<AObjectClass>(Hit.GetActor()))
		{
			float Dis = Hit.GetActor()->GetHorizontalDistanceTo(PlayerPawn);
			if (Dis <= PlayerPawn->GetDistance() && bCanDig)
			{
				bCanDig = false;
				GetWorldTimerManager().SetTimer(DigTimer, this, &AMyPlayerController::SetDigTimer, 0.5f, false, 0.5f);

				FVector ToT = Hit.GetActor()->GetActorLocation() - PlayerPawn->GetActorLocation();
				FRotator DiRo = ToT.Rotation();
				DiRo.Pitch = 0.0f;

				PlayerPawn->SetActorRotation(DiRo);
				PlayerPawn->ObjectDig(Hit.GetActor());

				return;
			}
		}
	}
}

void AMyPlayerController::R_Click()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		MyHUD->SetInventory(PlayerPawn->GetInven());
		Cast<AObjectClass>(Hit.GetActor())->Action(this);
	}
}

void AMyPlayerController::OpenInven()
{
	MyHUD->SetInven(PlayerPawn->GetInven());
	MyHUD->SwitchInventory();
}

void AMyPlayerController::Use_Inven_Swap()
{
	TArray<FInputActionKeyMapping> Binding = PlayerInput->GetKeysForAction("Use_Inven_Swap");
	for (auto It = Binding.CreateConstIterator(); It; It++) //아이템 제작 리스트
	{
		if (PlayerInput->GetKeyValue((*It).Key) != 0)
		{
			PlayerPawn->PickUseInven(It.GetIndex());
			return;
		}
	}
}

void AMyPlayerController::CameraRotation_L()
{
	PlayerPawn->CameraRotation_L();
}

void AMyPlayerController::CameraRotation_R()
{
	PlayerPawn->CameraRotation_R();
}

void AMyPlayerController::SetDigTimer()
{
	bCanDig = true;
}
