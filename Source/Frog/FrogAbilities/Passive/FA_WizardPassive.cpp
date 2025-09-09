// Fill out your copyright notice in the Description page of Project Settings.


#include "FA_WizardPassive.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"

void UFA_WizardPassive::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
		return;

	if (UAbilitySystemComponent* ASC = Cast<IAbilitySystemInterface>(ActorInfo->AvatarActor.Get())->GetAbilitySystemComponent())
	{
		EffectAppliedHandle = ASC->OnGameplayEffectAppliedDelegateToTarget
			.AddUObject(this, &UFA_WizardPassive::OnGameplayEffectApplied);
	}
}

void UFA_WizardPassive::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);

	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
		return;

	if (UAbilitySystemComponent* ASC = Cast<IAbilitySystemInterface>(ActorInfo->AvatarActor.Get())->GetAbilitySystemComponent())
	{
		ASC->OnGameplayEffectAppliedDelegateToTarget.Remove(EffectAppliedHandle);
	}
}

void UFA_WizardPassive::OnGameplayEffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectSpec)
{
	AActor* TargetActor = AbilitySystemComponent->GetOwnerActor();
	AActor* SourceActor = GameplayEffectSpec.GetContext().GetEffectCauser();

	if (SourceActor && SourceActor != TargetActor)
	{
		FGameplayTagContainer GrantedTags;
		GameplayEffectSpec.GetAllAssetTags(GrantedTags);

		if (TrackedTag.IsValid() && GrantedTags.HasTag(TrackedTag))
		{
			if (UAbilitySystemComponent* ASC = Cast<IAbilitySystemInterface>(GetActorInfo().AvatarActor.Get())->GetAbilitySystemComponent())
			{
				if (GainManaEffectClass)
				{
					FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
					Context.AddSourceObject(this);
					Context.AddInstigator(SourceActor->GetInstigator(), SourceActor->GetOwner()); 
					FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(GainManaEffectClass, 1.0f, Context);
                
					if (Spec.IsValid())
					{ 
						ASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), ASC);
					}
				}
			}
		}
	}
}
