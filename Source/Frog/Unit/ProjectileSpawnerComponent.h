// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Projectile.h"
#include "Components/ActorComponent.h"
#include "ProjectileSpawnerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FROG_API UProjectileSpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UProjectileSpawnerComponent();
	
	void RequestSpawnProjectile(const TSubclassOf<AProjectile>& ActorClass, const FVector& Location, const FRotator& Rotation,
								const FVector& FireDirection);
	
protected:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpawnProjectile(TSubclassOf<AProjectile> ActorClass, const FVector& Location,
	                                             const FRotator& Rotation, const FVector& FireDirection);
	
	void SpawnProjectileInternal(const TSubclassOf<AProjectile>& ActorClass, const FVector& Location,
	                             const FRotator& Rotation, FVector FireDirection, bool bApplyEffect) const;
	
};
