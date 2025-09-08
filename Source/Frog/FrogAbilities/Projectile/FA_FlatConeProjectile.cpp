// Fill out your copyright notice in the Description page of Project Settings.

#include "FA_FlatConeProjectile.h"

void UFA_FlatConeProjectile::FireProjectile()
{
	const FVector SpawnLocation = GetSpawnLocation();
	const FRotator BaseRotation = GetFireRotation();
	const FVector BaseDirection = GetFireDirection(SpawnLocation);
	
	if (NumProjectiles <= 1)
	{
		Super::FireProjectile();
		return;
	}
	
	const float AngleStep = SpreadAngle / (NumProjectiles - 1); // degrees
	const float HalfSpread = SpreadAngle / 2.f;
	
	for (int i = 0; i < NumProjectiles; i++)
	{
		float YawOffset = -HalfSpread + i * AngleStep;
		FRotator ProjectileRotation = BaseRotation;
		ProjectileRotation.Yaw += YawOffset;
		
		Super::FireProjectile(SpawnLocation, ProjectileRotation, BaseDirection);
	}
}

void UFA_FlatConeProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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
