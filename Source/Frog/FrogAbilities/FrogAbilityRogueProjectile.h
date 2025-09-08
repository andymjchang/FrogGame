// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogAbilities/FrogAbilityProjectile.h"
#include "FrogAbilityRogueProjectile.generated.h"

/**
 * 
 */
UCLASS()
class FROG_API UFrogAbilityRogueProjectile : public UFrogAbilityProjectile
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue Projectile Class")
	int32 NumProjectiles = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue Projectile Class")
	float SpreadAngle = 40;
private:
	virtual void FireProjectile() override;
};
