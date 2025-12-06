// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogAbilities/FrogAbility.h"
#include "Abilities/GameplayAbility.h"
#include "FA_Jump.generated.h"

/**
 * 
 */
UCLASS()
class FROG_API UFA_Jump : public UFrogAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
