// Fill out your copyright notice in the Description page of Project Settings.

#include "UnitAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Unit/UnitInterface.h"
#include "Net/UnrealNetwork.h"

void UUnitAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UUnitAttributeSet, Health);
	DOREPLIFETIME(UUnitAttributeSet, MaxHealth);
}

void UUnitAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
		if (NewValue <= 0.0f)
		{
			if (IUnitInterface* Owner = Cast<IUnitInterface>(GetOwningActor()))
			{
				Owner->HandleDeath();
			}
		}
	}
}

void UUnitAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float LocalDamage = GetDamage();
		SetDamage(0.f);
		if (LocalDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
		}
	}
}

void UUnitAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUnitAttributeSet, Health, OldHealth);
}

void UUnitAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUnitAttributeSet, MaxHealth, OldMaxHealth);
}

void UUnitAttributeSet::OnRep_MovementSpeedModifier(const FGameplayAttributeData& OldMovementSpeedModifier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUnitAttributeSet, MaxHealth, OldMovementSpeedModifier);
}
