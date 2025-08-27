// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyGameplayAbility.h"

UEnemyGameplayAbility::UEnemyGameplayAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
}