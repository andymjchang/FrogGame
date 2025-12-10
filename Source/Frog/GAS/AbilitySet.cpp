// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySet.h"

#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"

TArray<FGameplayAbilitySpecHandle> UAbilitySet::GrantAbilitiesToAbilitySystem(
	UAbilitySystemComponent* InputAbilitySystem)
{
	TArray<FGameplayAbilitySpecHandle> Handles;
	Handles.Reserve(AbilitiesSetList.Num());

	if (InputAbilitySystem) 
	{
		for (const auto AbilitySetItem: AbilitiesSetList)
		{
			FGameplayAbilitySpecHandle Handle = InputAbilitySystem->GiveAbility(
			   FGameplayAbilitySpec(
				  AbilitySetItem.GameplayAbility,
				  0,
				  static_cast<uint32>(AbilitySetItem.InputKey)
				  )
			   );
          
			Handles.AddUnique(Handle);
          
			// if (FrogHUD && Handle.IsValid())
			// {
			// 	FrogHUD->AssignAbilityToUISlot(Handle, InputAbilitySystem);
			// }
		}
	}
    
	return Handles;
}
