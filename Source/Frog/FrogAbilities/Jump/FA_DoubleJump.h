// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FA_Jump.h"
#include "FA_DoubleJump.generated.h"

/**
 * 
 */
UCLASS()
class FROG_API UFA_DoubleJump : public UFA_Jump
{
	GENERATED_BODY()
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};