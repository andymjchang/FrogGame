// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogAbilities/FrogAbilityPrimary.h"

#include "FrogCharacter/FrogCharacter.h"
#include "Unit/ProjectileSpawnerComponent.h"

void UFrogAbilityPrimary::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbilityCost(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
    
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	IUnitInterface* Unit = Cast<IUnitInterface>(ActorInfo->AvatarActor.Get());
	if (!Unit)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (UProjectileSpawnerComponent* ProjectileSpawner = Unit->GetProjectileSpawnerComponent())
	{
		const FVector UnitLocation = ActorInfo->AvatarActor.Get()->GetActorLocation() + FVector(0, 0, 50.f);
		const FVector FireDirection = (GetCrosshairLocation(false) - UnitLocation).GetSafeNormal();
		const FRotator FireRotation = FRotator(0, 0, 0);
		// const FRotator FireRotation = FireDirection.Rotation();
		ProjectileSpawner->RequestSpawnProjectile(ProjectileClass, UnitLocation, FireRotation, FireDirection);
	}

	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
