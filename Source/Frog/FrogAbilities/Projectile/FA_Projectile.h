// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogAbilities/FrogAbility.h"
#include "FA_Projectile.generated.h"

class AProjectile;
/**
 * 
 */
UCLASS()
class FROG_API UFA_Projectile : public UFrogAbility
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Class")
	TSubclassOf<AProjectile> ProjectileClass;

	virtual FVector GetSpawnLocation();
	virtual FRotator GetFireRotation();
	virtual FVector GetFireDirection(FVector SpawnLocation);
	virtual USceneComponent* GetTargetComponent();
	void FireProjectile(const FVector& SpawnLocation, const FRotator& ProjectileRotation, const FVector& FireDirection);
	virtual void FireProjectile();
};
