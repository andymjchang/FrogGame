// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyGameplayAbility.h"
#include "EnemyTestAbility.generated.h"

class AProjectile;
/**
 * 
 */
UCLASS()
class FROG_API UEnemyTestAbility : public UEnemyGameplayAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                     FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AProjectile> ProjectileClass;
};
