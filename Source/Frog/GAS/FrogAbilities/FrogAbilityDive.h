// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogGameplayAbility.h"
#include "Abilities/GameplayAbility.h"
#include "FrogAbilityDive.generated.h"

/**
 * 
 */
UCLASS()
class FROG_API UFrogAbilityDive : public UFrogGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
								const FGameplayAbilityActorInfo* ActorInfo,
								const FGameplayAbilityActivationInfo ActivationInfo,
								const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
						   const FGameplayAbilityActorInfo* ActorInfo,
						   const FGameplayAbilityActivationInfo ActivationInfo,
						   bool bReplicateEndAbility,
						   bool bWasCancelled) override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle,
							  const FGameplayAbilityActorInfo* ActorInfo,
							  const FGameplayAbilityActivationInfo ActivationInfo) override;
};
