// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FrogGameplayAbility.generated.h"

/**
 * CooldownDuration and CooldownTags allow us to use one GE BP for cooldowns,
 * with different cooldown tags set in each GA BP.
 */
UCLASS()
class FROG_API UFrogGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UFrogGameplayAbility();
	virtual const FGameplayTagContainer* GetCooldownTags() const override;
	virtual void ApplyCooldown(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                           FGameplayAbilityActivationInfo ActivationInfo) const override;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldown")
	FScalableFloat CooldownDuration;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldown")
	FGameplayTagContainer CooldownTags;

	// Temp container that we will return the pointer to in GetCooldownTags().
	// This will be a union of our CooldownTags and the Cooldown GE's cooldown tags.
	UPROPERTY(Transient)
	FGameplayTagContainer TempCooldownTags;
};
