// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogAbilityGrapple.h"

#include "FrogCharacter.h"

void UFrogAbilityGrapple::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (AFrogCharacter* Character = Cast<AFrogCharacter>(ActorInfo->AvatarActor.Get()))
	{
		Character->HandleGrapple();
	}

	
}

void UFrogAbilityGrapple::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	
	if (AFrogCharacter* Character = Cast<AFrogCharacter>(ActorInfo->AvatarActor.Get()))
	{
		Character->HandleStopGrapple();
	}
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
