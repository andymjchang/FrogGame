// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogAbilities/FrogAbilityPrimary.h"

#include "FrogCharacter/FrogCharacter.h"

void UFrogAbilityPrimary::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbilityCost(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
    
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FireProjectile();

	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
