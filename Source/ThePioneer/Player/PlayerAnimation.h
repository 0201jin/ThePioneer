// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimation.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateDead, const bool, Dead);

UCLASS()
class THEPIONEER_API UPlayerAnimation : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	void CharacterDead(bool _Dead);

protected:
	UPROPERTY(BluePrintAssignable, Category = "Character")
		FUpdateDead DeadUpdate;
};
