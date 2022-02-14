// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainCharacterAnim.generated.h"

class AMainCharacter;
/**
 * 
 */
UCLASS()
class OURRPG_API UMainCharacterAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category=AnimationProperties)
	void UpdateAnimationProperties();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsRunning;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool IsFalling;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	AMainCharacter* Character;
};
