// Fill out your copyright notice in the Description page of Project Settings.

#include "FrogAbilityDoubleJump.h"
#include "FrogCharacter/FrogCharacter.h"

void UFrogAbilityDoubleJump::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	if (AFrogCharacter* FrogCharacter = Cast<AFrogCharacter>(ActorInfo->AvatarActor.Get()))
	{
		FrogCharacter->JumpMaxCount++;
	}
}

void UFrogAbilityDoubleJump::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	if (AFrogCharacter* FrogCharacter = Cast<AFrogCharacter>(ActorInfo->AvatarActor.Get()))
	{
		FrogCharacter->JumpMaxCount = FMath::Max(1, FrogCharacter->JumpMaxCount - 1);
	}
    
	Super::OnRemoveAbility(ActorInfo, Spec);
}