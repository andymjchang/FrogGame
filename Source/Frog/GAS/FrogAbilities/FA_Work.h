// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FA_Work.generated.h"

/**
 * 
 */
UCLASS()
class FROG_API UFA_Work : public UGameplayAbility
{
	GENERATED_BODY()

public:
    UFA_Work();
	virtual void ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
									 FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void HandleInputReleased(float TimeHeld);
};
