// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTestAbility.h"

#include "Unit/ProjectileSpawnerComponent.h"
#include "Unit/UnitInterface.h"

void UEnemyTestAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
	// if (!CommitAbilityCost(Handle, ActorInfo, ActivationInfo))
	// {
	// 	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	// 	return;
	// }
    
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	IUnitInterface* Unit = Cast<IUnitInterface>(ActorInfo->AvatarActor.Get());
	if (!Unit)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (UProjectileSpawnerComponent* ProjectileSpawner = Unit->GetProjectileSpawnerComponent())
	{
		FVector UnitLocation = ActorInfo->AvatarActor.Get()->GetActorLocation();
		ProjectileSpawner->RequestSpawnProjectile(ProjectileClass,
			UnitLocation, FRotator::ZeroRotator, FVector(1, 0 , 0));
	}

	// CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
