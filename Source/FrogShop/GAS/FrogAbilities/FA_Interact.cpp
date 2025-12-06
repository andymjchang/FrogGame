// Fill out your copyright notice in the Description page of Project Settings.


#include "FA_Interact.h"

#include "FrogShop/FrogCharacter/FrogCharacter.h"


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
		Frog->Interact();
	}
}

void UFA_Interact::InputPressed(const FGameplayAbilitySpecHandle Handle,
									   const FGameplayAbilityActorInfo* ActorInfo,
									   const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	if (GEngine)
	{
		const FString DebugMessage = FString::Printf(TEXT("ACTIVATING"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMessage);
	}
}

void UFA_Interact::InputReleased(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
