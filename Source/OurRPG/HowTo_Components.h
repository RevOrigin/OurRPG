// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CollidingPawnMovementComponent.h"
#include "HowTo_Components.generated.h"


UCLASS()
class OURRPG_API AHowTo_Components : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHowTo_Components();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
		class UParticleSystemComponent* OurParticleSystem;
	
	UPROPERTY()
		class UCollidingPawnMovementComponent* OurMovementComponent;
	
	virtual UPawnMovementComponent* GetMovementComponent() const override;
	
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Turn(float AxisValue);
	void ParticleToggle();
};