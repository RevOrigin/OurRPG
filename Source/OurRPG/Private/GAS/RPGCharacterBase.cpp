// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/RPGCharacterBase.h"

// Sets default values
ARPGCharacterBase::ARPGCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARPGCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARPGCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARPGCharacterBase::PossessedBy(AController* NewController)
{
	
}

void ARPGCharacterBase::UnPossessed()
{
	
}

void ARPGCharacterBase::OnRep_Controller()
{
	
}

float ARPGCharacterBase::GetHealth() const
{
	return .0f;
}

float ARPGCharacterBase::GetMaxHealth() const
{
	return  .0f;
}

UAbilitySystemComponent* ARPGCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARPGCharacterBase::HandleDamage(float Damage, const FHitResult& HitInfo)
{
	
}

void ARPGCharacterBase::HandleHealthChanged(float DeltaValue)
{
	
}

void ARPGCharacterBase::OnHealthChanged(float DeltaValue)
{
	
}

void ARPGCharacterBase::OnDamaged(float Damage, const FHitResult& HitInfo)
{
	
}











