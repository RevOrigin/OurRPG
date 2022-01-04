// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/RPGAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "GAS/RPGCharacterBase.h"
class ARPGCharacterBase;

URPGAttributeSet::URPGAttributeSet()
	: Health(1.f)
	, MaxHealth(1.f)
{
	
}

void URPGAttributeSet::OnRepHealth(const FGameplayAttributeData& OldValue)
{
	
}

void URPGAttributeSet::OnRepMaxHealth(const FGameplayAttributeData& OldValue)
{
	
}

void URPGAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	
}



void URPGAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	// 当改变的属性是最大生命值时，需要调整
	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
}

// 影响属性的gameplayEffect执行后被调用
void URPGAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// 获取当前effect的句柄
	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	// 获取该Effect所对应的system
	UAbilitySystemComponent* Src = Context.GetOriginalInstigatorAbilitySystemComponent();
	// 
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	// 计算old和new的差值
	float DeltaValue = 0;
	// EvaluatedData代表了该属性变化过程中的值
	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		// 如果是加的操作，则直接用变化值的大小作为差值
		DeltaValue = Data.EvaluatedData.Magnitude;
	}
	// 找到该属性值的目标
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	ARPGCharacterBase* TargetCharacter = nullptr;
	// 此处的Target值即代表了该属性即将应用的对象
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid()) 
	{
		// 此处获取的是物理actor（即控制动画等物理表现的actor）
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		// 此处获取的是player controller
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<ARPGCharacterBase>(TargetActor);
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// 对于伤害值，需要考虑其来源来计算最终的伤害
		AActor* SrcActor = nullptr;
		AController* SrcController = nullptr;
		ARPGCharacterBase* SrcCharacter = nullptr;
		if (Src && Src->AbilityActorInfo.IsValid() && Src->AbilityActorInfo->AvatarActor.IsValid())
		{
			SrcActor = Src->AbilityActorInfo->AvatarActor.Get();
			SrcController = Src->AbilityActorInfo->PlayerController.Get();
			if (SrcController == nullptr && SrcActor != nullptr)
			{
				// 假如来源不存在对应的controller，需要手动获取
				if (APawn* Pawn = Cast<APawn>(SrcActor))
				{
					SrcController = Pawn->GetController();
				}
			}
			if (SrcController)
			{
				SrcCharacter = Cast<ARPGCharacterBase>(SrcController->GetPawn());
			}
			else
			{
				SrcCharacter = Cast<ARPGCharacterBase>(SrcActor);
			}

			// 直接检查context是否存有对应的来源
			// 如果有，直接使用该值作为SrcActor
			if (Context.GetEffectCauser())
			{
				SrcActor = Context.GetEffectCauser();
			}
		}

		// 找到击中的结果
		FHitResult HitResult;
		if (Context.GetHitResult())
		{
			HitResult = *Context.GetHitResult();
		}

		// 存储临时的damage值
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		if (LocalDamageDone > 0)
		{
			// 计算伤害值并计算生命值
			const float OldHealth = GetHealth();
			// clamp 防止生命值超出范围
			SetHealth(FMath::Clamp(OldHealth - LocalDamageDone, .0f, GetMaxHealth()));

			if (TargetCharacter)
			{
				// 处理实际的伤害
				TargetCharacter->HandleDamage(LocalDamageDone, HitResult);
			}
		}
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// 处理生命值
		SetHealth(FMath::Clamp(GetHealth(), .0f, GetMaxHealth()));
		if (TargetCharacter)
		{
			TargetCharacter->HandleHealthChanged(DeltaValue);
		}
	}
}

