// Fill out your copyright notice in the Description page of Project Settings.


#include "FA_Interact.h"

#include "FrogCharacter/FrogCharacter.h"


void UFA_Interact::ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
 	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (AFrogCharacter* Frog = Cast<AFrogCharacter>(ActorInfo->AvatarActor.Get()))
	{
		Frog->PlayerStartInteract();
	}
}

void UFA_Interact::InputReleased(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	
	if (AFrogCharacter* Frog = Cast<AFrogCharacter>(ActorInfo->AvatarActor.Get()))
	{
		Frog->PlayerStopInteract();
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
