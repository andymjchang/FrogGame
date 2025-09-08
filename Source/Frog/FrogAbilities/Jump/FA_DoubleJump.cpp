// Fill out your copyright notice in the Description page of Project Settings.

#include "FA_DoubleJump.h"
#include "FrogCharacter/FrogCharacter.h"

void UFA_DoubleJump::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	if (AFrogCharacter* FrogCharacter = Cast<AFrogCharacter>(ActorInfo->AvatarActor.Get()))
	{
		FrogCharacter->JumpMaxCount++;
	}
}

void UFA_DoubleJump::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	if (AFrogCharacter* FrogCharacter = Cast<AFrogCharacter>(ActorInfo->AvatarActor.Get()))
	{
		FrogCharacter->JumpMaxCount = FMath::Max(1, FrogCharacter->JumpMaxCount - 1);
	}
    
	Super::OnRemoveAbility(ActorInfo, Spec);
}