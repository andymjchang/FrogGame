// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySet.h"

#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"

TArray<FGameplayAbilitySpecHandle> UAbilitySet::GrantAbilitiesToAbilitySystem(
	UAbilitySystemComponent* AbilitySystemComponent) const
{
	check(AbilitySystemComponent);

	TArray<FGameplayAbilitySpecHandle> handles;
	handles.Reserve(AbilitiesSetList.Num());

	for (const auto abilitySetItem: AbilitiesSetList)
	{
		handles.AddUnique(AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(
				abilitySetItem.GameplayAbility,
				0,
				static_cast<uint32>(abilitySetItem.InputKey)
				)
			));
	}

	return handles;
}
