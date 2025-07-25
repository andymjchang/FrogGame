// Fill out your copyright notice in the Description page of Project Settings.

#include "FrogAbilityJump.h"
#include "GameFramework/Character.h"
#include "FrogAbilitySystem.h"

void UFrogAbilityJump::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbilityCost(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get()))
	{
		Character->Jump();
	}
	
	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

