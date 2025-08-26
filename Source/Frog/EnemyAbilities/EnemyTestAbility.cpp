// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTestAbility.h"

#include "EnemyCharacter/EnemyCharacter.h"
#include "Unit/ProjectileSpawnerComponent.h"

void UEnemyTestAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbilityCost(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
    
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AEnemyCharacter* EnemyUnit = Cast<AEnemyCharacter>(ActorInfo->AvatarActor.Get());
	if (!EnemyUnit)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (UProjectileSpawnerComponent* ProjectileSpawner = EnemyUnit->GetProjectileSpawnerComponent())
	{
		ProjectileSpawner->RequestSpawnProjectile(ProjectileClass,
			EnemyUnit->GetActorLocation(), FRotator::ZeroRotator, FVector(0, 0 , 1));
	}

	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
