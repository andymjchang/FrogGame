#include "FrogAbilityDive.h"

#include "FrogCharacter/FrogCharacter.h"

void UFrogAbilityDive::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (AFrogCharacter* Character = Cast<AFrogCharacter>(ActorInfo->AvatarActor.Get()))
	{
		Character->Crouch();
	}
}

void UFrogAbilityDive::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (AFrogCharacter* Character = Cast<AFrogCharacter>(ActorInfo->AvatarActor.Get()))
	{
		Character->UnCrouch();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UFrogAbilityDive::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
