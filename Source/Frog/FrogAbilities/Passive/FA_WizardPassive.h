// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogAbilities/FrogAbility.h"
#include "FA_WizardPassive.generated.h"

/**
 * 
 */
UCLASS()
class FROG_API UFA_WizardPassive : public UFrogAbility
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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GainManaEffectClass;
};
