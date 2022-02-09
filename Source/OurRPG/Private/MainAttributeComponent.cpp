// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAttributeComponent.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

// Sets default values for this component's properties
UMainAttributeComponent::UMainAttributeComponent()
{
	HealthMax = 100.f;
	Health = HealthMax;

	Rage = 0;
	RageMax = 100;

	SetIsReplicatedByDefault(true);
}

bool UMainAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

bool UMainAttributeComponent::IsAlive() const
{
	return Health > .0f;
}

bool UMainAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

float UMainAttributeComponent::GetHealth() const
{
	return Health;
}

float UMainAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

bool UMainAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < .0f)
	{
		return false;
	}

	if (Delta < .0f)
	{
		const float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, .0f, HealthMax);

	float ActualDelta = NewHealth - OldHealth;

	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;

		if (ActualDelta != .0f)
		{
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
		}

		if (ActualDelta < .0f && Health == .0f) 
		{
			//todo:结束游戏
		}
	}

	return ActualDelta != 0;
}

float UMainAttributeComponent::GetRage() const
{
	return Rage;
}

bool UMainAttributeComponent::ApplyRage(AActor* InstigatorActor, float Delta)
{
	float OldRage = Rage;

	Rage = FMath::Clamp(Rage + Delta, .0f, RageMax);

	float ActualDelta = Rage - OldRage;
	if (ActualDelta != .0f)
	{
		OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta);
	}
	return ActualDelta != 0;
}

UMainAttributeComponent* UMainAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UMainAttributeComponent>(FromActor);
	}
	return nullptr;
}

bool UMainAttributeComponent::IsActorAlive(AActor* Actor)
{
	UMainAttributeComponent* AttributeCom = GetAttributes(Actor);
	if (AttributeCom)
	{
		return AttributeCom->IsAlive();
	}
	return false;
}

void UMainAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void UMainAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
}

void UMainAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMainAttributeComponent, Health);
	DOREPLIFETIME(UMainAttributeComponent, HealthMax);

	DOREPLIFETIME(UMainAttributeComponent, Rage);
	DOREPLIFETIME(UMainAttributeComponent, RageMax);
}












