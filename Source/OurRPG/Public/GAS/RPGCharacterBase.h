// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/RPGAttributeSet.h"
#include "Abilities/RPGAbilitySystemComponent.h"
#include "RPGCharacterBase.generated.h"

UCLASS()
class OURRPG_API ARPGCharacterBase : public ACharacter,
public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGCharacterBase();

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void OnRep_Controller() override;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;


public:
	virtual float GetHealth() const;

	virtual float GetMaxHealth() const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	URPGAttributeSet* AttributeSet;

	URPGAbilitySystemComponent* AbilitySystemComponent;

	bool bAbilityInitialized;


protected:
	/**
	 * @param Damage 伤害值（未放缩）
	 * @param HitInfo 产生此次伤害的碰撞信息
	 */
	void OnDamaged(float Damage, const FHitResult& HitInfo);

	/**
	 * @param DeltaValue 生命值的变化量
	 */
	void OnHealthChanged(float DeltaValue);

	virtual void HandleDamage(float Damage, const FHitResult& HitInfo);
	virtual void HandleHealthChanged(float DeltaValue);

	friend URPGAttributeSet;
};
