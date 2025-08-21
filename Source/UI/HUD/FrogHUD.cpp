// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogHUD.h"
#include "AbilityIcon.h"
#include "Abilities/GameplayAbility.h"

void UFrogHUD::AssignAbilityToUISlot(UGameplayAbility* InputAbility, UAbilitySystemComponent* InputAbilitySystem) const
{
	const FGameplayTagContainer& AbilityTags = InputAbility->GetAssetTags();

	if (AbilityTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Frog.AbilitySlot.Primary"))))
	{
		if (AbilityIconPrimary)
		{
			AbilityIconPrimary->TrackAbility(InputAbilitySystem, InputAbility);
		}
	}
	else if (AbilityTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Frog.AbilitySlot.Secondary"))))
	{
		if (AbilityIconSecondary)
		{
			AbilityIconSecondary->TrackAbility(InputAbilitySystem, InputAbility);
		}
	}
	else if (AbilityTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Frog.AbilitySlot.Grapple"))))
	{
		if (AbilityIconGrapple)
		{
			AbilityIconGrapple->TrackAbility(InputAbilitySystem, InputAbility);
		}
	}
	else if (AbilityTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Frog.AbilitySlot.Dive"))))
	{
		if (AbilityIconDive)
		{
			AbilityIconDive->TrackAbility(InputAbilitySystem, InputAbility);
		}
	}
	else if (AbilityTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Frog.AbilitySlot.Utility"))))
	{
		if (AbilityIconUtility)
		{
			AbilityIconUtility->TrackAbility(InputAbilitySystem, InputAbility);
		}
	}
}
