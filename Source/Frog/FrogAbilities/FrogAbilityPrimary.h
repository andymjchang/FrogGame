// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogAbilityProjectile.h"
#include "FrogGameplayAbility.h"
#include "FrogAbilityPrimary.generated.h"

class AProjectile;
/**
 * 
 */
UCLASS()
class FROG_API UFrogAbilityPrimary : public UFrogAbilityProjectile
{
	GENERATED_BODY()

public: 
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
