// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/FrogAbilities/FrogAbilityPrimary.h"

#include "FrogCharacter/FrogCharacter.h"

void UFrogAbilityPrimary::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbilityCost(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
    
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AFrogCharacter* Character = Cast<AFrogCharacter>(ActorInfo->AvatarActor.Get());
	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	Character->SpawnProjectile(ProjectileClass, Character->GetActorLocation(), FRotator::ZeroRotator);

	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
