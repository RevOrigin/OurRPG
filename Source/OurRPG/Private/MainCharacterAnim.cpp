// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterAnim.h"

#include "MainCharacter.h"

void UMainCharacterAnim::NativeInitializeAnimation()
{
	if (Character == nullptr)
	{
		auto* Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			Character = Cast<AMainCharacter>(Pawn);
		}
	}
}

void UMainCharacterAnim::UpdateAnimationProperties()
{
	if (Character == nullptr)
	{
		auto* Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			Character = Cast<AMainCharacter>(Pawn);
		}
	}
}

