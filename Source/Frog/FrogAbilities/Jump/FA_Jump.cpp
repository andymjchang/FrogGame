// Fill out your copyright notice in the Description page of Project Settings.

#include "FA_Jump.h"
#include "GameFramework/Character.h"
#include "GAS/FrogAbilitySystem.h"

void UFA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get()))
	{
		Character->UnCrouch();
		Character->Jump();
	}
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

