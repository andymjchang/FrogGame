// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogHUD.h"

#include "AbilityIcon.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Components/RadialSlider.h"
#include "Components/TextBlock.h"

void UFrogHUD::AssignAbilityToUISlot(FGameplayAbilitySpecHandle InputAbilitySpecHandle, UAbilitySystemComponent* InputAbilitySystem) const
{
	FGameplayAbilitySpec* AbilitySpec = InputAbilitySystem->FindAbilitySpecFromHandle(InputAbilitySpecHandle);
	const FGameplayTagContainer& AbilityTags = AbilitySpec->Ability->GetAssetTags();

	if (AbilityTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Frog.AbilitySlot.Primary"))))
	{
		if (AbilityIconPrimary)
		{
			AbilityIconPrimary->TrackAbility(InputAbilitySystem, InputAbilitySpecHandle);
		}
	}
	else if (AbilityTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Frog.AbilitySlot.Secondary"))))
	{
		if (AbilityIconSecondary)
		{
			AbilityIconSecondary->TrackAbility(InputAbilitySystem, InputAbilitySpecHandle);
		}
	}
	else if (AbilityTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Frog.AbilitySlot.Grapple"))))
	{
		if (AbilityIconGrapple)
		{
			AbilityIconGrapple->TrackAbility(InputAbilitySystem, InputAbilitySpecHandle);
		}
	}
	else if (AbilityTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Frog.AbilitySlot.Dive"))))
	{
		if (AbilityIconDive)
		{
			AbilityIconDive->TrackAbility(InputAbilitySystem, InputAbilitySpecHandle);
		}
	}
	else if (AbilityTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Frog.AbilitySlot.Utility"))))
	{
		if (AbilityIconUtility)
		{
			AbilityIconUtility->TrackAbility(InputAbilitySystem, InputAbilitySpecHandle);
		}
	}
}

void UFrogHUD::UpdateMana(const float NewManaValue)
{
	Mana = NewManaValue;
	UpdateManaHUD();
}

void UFrogHUD::UpdateMaxMana(const float NewMaxManaValue)
{
	MaxMana = NewMaxManaValue;

	if (MaxMana > 0.0f)
	{
		ManaBar->SetVisibility(ESlateVisibility::Visible);
		ManaText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ManaBar->SetVisibility(ESlateVisibility::Hidden);
		ManaText->SetVisibility(ESlateVisibility::Hidden);
	}
	
	UpdateManaHUD();
}

void UFrogHUD::UpdateManaHUD()
{
	if (!ManaBar || !ManaText) return;
	
	if (MaxMana > 0.f)
	{
		ManaBar->SetValue(Mana / MaxMana);
		ManaText->SetText(FText::AsNumber(Mana));
	}
}

