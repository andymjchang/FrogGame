// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogHUD.h"
#include "AbilityIcon.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"

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
