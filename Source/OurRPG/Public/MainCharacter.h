// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponActor.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UAnimMontage;
class UMainAttributeComponent;
class UParticleSystem;

UCLASS()
class OURRPG_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	//! 该属性表示被击中时展示的属性名
	UPROPERTY(VisibleAnywhere, Category="Effects")
	FName TimeToHitParamName;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UMainAttributeComponent* AttributeComponent;

	// 武器类型
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeaponActor> WeaponClass = AWeaponActor::StaticClass();

	// 当前的武器
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AWeaponActor* CurrentWeapon;
	
	void MoveForward(float Value);
	void MoveRight(float Value);

	void FlyStart();
	void FlyStop();

	void Attack();

	void Dash();

	void PrimiryInteract();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UMainAttributeComponent* OwningCom, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;;

	virtual FVector GetPawnViewLocation() const override;
	
	
public:
	// Sets default values for this character's properties
	AMainCharacter();
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;
	
	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);
};
