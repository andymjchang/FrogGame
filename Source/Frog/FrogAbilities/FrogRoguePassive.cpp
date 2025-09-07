#include "FrogAbilities/FrogRoguePassive.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"

UFrogRoguePassive::UFrogRoguePassive()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}


void UFrogRoguePassive::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
		return;

	if (UAbilitySystemComponent* ASC = Cast<IAbilitySystemInterface>(ActorInfo->AvatarActor.Get())->GetAbilitySystemComponent())
	{
		// Bind the delegate and store the handle
		EffectAppliedHandle = ASC->OnGameplayEffectAppliedDelegateToTarget
			.AddUObject(this, &UFrogRoguePassive::OnGameplayEffectApplied);
	}
}

void UFrogRoguePassive::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);

	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
		return;

	if (UAbilitySystemComponent* ASC = Cast<IAbilitySystemInterface>(ActorInfo->AvatarActor.Get())->GetAbilitySystemComponent())
	{
		// Unbind the delegate using the stored handle
		ASC->OnGameplayEffectAppliedDelegateToTarget.Remove(EffectAppliedHandle);
	}
}

void UFrogRoguePassive::OnGameplayEffectApplied(
	UAbilitySystemComponent* TargetASC,
	const FGameplayEffectSpec& SpecApplied,
	FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	if (GrantedTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Debuff"))))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
			FString::Printf(TEXT("%s got DEBUFF from %s"),
				*TargetASC->GetOwner()->GetName(),
				*SpecApplied.Def->GetName()));
	}
}
