// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogGameplayAbility.h"
#include "FrogAbilityProjectile.generated.h"

class AProjectile;
/**
 * 
 */
UCLASS()
class FROG_API UFrogAbilityProjectile : public UFrogGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Class")
	TSubclassOf<AProjectile> ProjectileClass;

	virtual FVector GetSpawnLocation();
	virtual FRotator GetFireRotation();
	virtual FVector GetFireDirection(FVector SpawnLocation);
	virtual USceneComponent* GetTargetComponent();
	void FireProjectile(FVector SpawnLocation, FRotator ProjectileRotation, FVector FireDirection);
	virtual void FireProjectile();
};
