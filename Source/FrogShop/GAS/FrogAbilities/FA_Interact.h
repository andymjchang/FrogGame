// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FA_Interact.generated.h"

/**
 * CooldownDuration and CooldownTags allow us to use one GE BP for cooldowns,
 * with different cooldown tags set in each GA BP.
 */
UCLASS()
class FROGSHOP_API UFA_Interact : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                  FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void InputReleased(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                           FGameplayAbilityActivationInfo ActivationInfo) override;
protected:
	

private:
	
};