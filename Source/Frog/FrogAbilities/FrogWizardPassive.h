// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogGameplayAbility.h"
#include "FrogWizardPassive.generated.h"

/**
 * 
 */
UCLASS()
class FROG_API UFrogWizardPassive : public UFrogGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	void OnGameplayEffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
	                             const FGameplayEffectSpec& GameplayEffectSpec,
	                             FActiveGameplayEffectHandle ActiveGameplayEffectSpec);

	FDelegateHandle EffectAppliedHandle;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TrackedTag;
};
