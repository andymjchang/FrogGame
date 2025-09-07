// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogWizardPassive.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"

void UFrogWizardPassive::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
		return;

	if (UAbilitySystemComponent* ASC = Cast<IAbilitySystemInterface>(ActorInfo->AvatarActor.Get())->GetAbilitySystemComponent())
	{
		EffectAppliedHandle = ASC->OnGameplayEffectAppliedDelegateToTarget
			.AddUObject(this, &UFrogWizardPassive::OnGameplayEffectApplied);
	}
}

void UFrogWizardPassive::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);

	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
		return;

	if (UAbilitySystemComponent* ASC = Cast<IAbilitySystemInterface>(ActorInfo->AvatarActor.Get())->GetAbilitySystemComponent())
	{
		ASC->OnGameplayEffectAppliedDelegateToTarget.Remove(EffectAppliedHandle);
	}
}

void UFrogWizardPassive::OnGameplayEffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectSpec)
{
	AActor* TargetActor = AbilitySystemComponent->GetOwnerActor();
	AActor* SourceActor = GameplayEffectSpec.GetContext().GetEffectCauser();

	if (SourceActor && SourceActor != TargetActor)
	{
		FGameplayTagContainer GrantedTags;
		GameplayEffectSpec.GetAllGrantedTags(GrantedTags);

		if (GrantedTags.HasTag(TrackedTag))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("x: %d"), 13));
		}
	}
}
