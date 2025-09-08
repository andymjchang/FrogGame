#include "FrogAbilities/Passive/FA_RoguePassive.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"

UFA_RoguePassive::UFA_RoguePassive()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}


void UFA_RoguePassive::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
		return;

	if (UAbilitySystemComponent* ASC = Cast<IAbilitySystemInterface>(ActorInfo->AvatarActor.Get())->GetAbilitySystemComponent())
	{
		// Bind the delegate and store the handle
		EffectAppliedHandle = ASC->OnGameplayEffectAppliedDelegateToTarget
			.AddUObject(this, &UFA_RoguePassive::OnGameplayEffectApplied);
	}
}

void UFA_RoguePassive::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
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

void UFA_RoguePassive::OnGameplayEffectApplied(
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
