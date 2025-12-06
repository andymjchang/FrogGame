// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogAbilities/Projectile/FA_Projectile.h"
#include "FA_FlatConeProjectile.generated.h"

/**
 * 
 */
UCLASS()
class FROG_API UFA_FlatConeProjectile : public UFA_Projectile
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue Projectile Class")
	int32 NumProjectiles = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue Projectile Class")
	float SpreadAngle = 40;

	virtual void ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
private:
	virtual void FireProjectile() override;
};
