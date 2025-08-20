// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySet.h"

#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"

TArray<FGameplayAbilitySpecHandle> UAbilitySet::GrantAbilitiesToAbilitySystem(
	UAbilitySystemComponent* AbilitySystemComponent) const
{
	check(AbilitySystemComponent);

	TArray<FGameplayAbilitySpecHandle> Handles;
	Handles.Reserve(AbilitiesSetList.Num());

	for (const auto AbilitySetItem: AbilitiesSetList)
	{
		Handles.AddUnique(AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(
				AbilitySetItem.GameplayAbility,
				0,
				static_cast<uint32>(AbilitySetItem.InputKey)
				)
			));
	}

	return Handles;
}
