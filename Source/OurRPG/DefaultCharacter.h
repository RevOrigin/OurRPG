// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DefaultCharacter.generated.h"

UCLASS()
class OURRPG_API ADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADefaultCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void MoveForward(float Value);
	
	UFUNCTION()
		void MoveRight(float Value);

	UFUNCTION()
		void StartJump();
	
	UFUNCTION()
		void StopJump();

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* CharacterCameraComponent;

	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* SpringArmComponent;

private:
	void PitchCamera(float value);
	void YawCamera(float value);
	void ZoomIn();
	void ZoomOut();


	FVector2D CameraInput;
	float ZoomFactor;
};
