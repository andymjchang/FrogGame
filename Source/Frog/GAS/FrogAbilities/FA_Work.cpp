// Fill out your copyright notice in the Description page of Project Settings.


#include "FA_Work.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "FrogCharacter/FrogCharacter.h"
#include "FrogGameplay/Stations/WorkStation.h"

UFA_Work::UFA_Work()
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFA_Work::ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
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
		if (AWorkStation* WorkStation = Cast<AWorkStation>(Frog->GetClosestInteractable()))
		{
			if (HasAuthority(&ActivationInfo))
			{
				WorkStation->StartWork(Frog->GetPlayerState());
			}
		}
	}

	UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
	WaitInputReleaseTask->OnRelease.AddDynamic(this, &UFA_Work::HandleInputReleased);
	WaitInputReleaseTask->ReadyForActivation();
}

void UFA_Work::HandleInputReleased(float TimeHeld)
{
	if (AFrogCharacter* Frog = Cast<AFrogCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (AWorkStation* WorkStation = Cast<AWorkStation>(Frog->GetClosestInteractable()))
		{
			if (HasAuthority(&CurrentActivationInfo))
			{
				WorkStation->StopWork(Frog->GetPlayerState());
			}
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
