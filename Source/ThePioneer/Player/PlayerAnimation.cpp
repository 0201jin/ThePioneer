// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimation.h"

void UPlayerAnimation::CharacterDead(bool _Dead)
{
	DeadUpdate.Broadcast(_Dead);
}