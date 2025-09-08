// Fill out your copyright notice in the Description page of Project Settings.

#include "FrogAbilityRogueProjectile.h"

void UFrogAbilityRogueProjectile::FireProjectile()
{
	// Super::FireProjectile();
	const FVector SpawnLocation = GetSpawnLocation();
	const FRotator BaseRotation = GetFireRotation();
	const FVector BaseDirection = GetFireDirection(SpawnLocation);
	
	if (NumProjectiles <= 1)
	{
		// fallback to single projectile
		Super::FireProjectile();
		return;
	}
	
	// Spread projectiles evenly around BaseRotation.Yaw
	const float AngleStep = SpreadAngle / (NumProjectiles - 1); // degrees
	const float HalfSpread = SpreadAngle / 2.f;
	
	for (int i = 0; i < NumProjectiles; i++)
	{
		// Compute the rotation offset for this projectile
		float YawOffset = -HalfSpread + i * AngleStep;
		FRotator ProjectileRotation = BaseRotation;
		ProjectileRotation.Yaw += YawOffset;
	
		// Convert rotation to direction
		FVector ProjectileDirection = (ProjectileRotation.Vector() * BaseDirection).GetSafeNormal();
	
		// Fire projectile with modified rotation/direction
		Super::FireProjectile(SpawnLocation, ProjectileRotation, BaseDirection);
	}
}

void UFrogAbilityRogueProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
										  const FGameplayAbilityActorInfo* ActorInfo,
										  const FGameplayAbilityActivationInfo ActivationInfo,
										  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbilityCost(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	FireProjectile();
	
	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false);
}
