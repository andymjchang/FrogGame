// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FA_Projectile.h"
#include "FA_BasicProjectile.generated.h"

class AProjectile;
/**
 * 
 */
UCLASS()
class FROG_API UFA_BasicProjectile : public UFA_Projectile
{
	GENERATED_BODY()

public: 
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
