#pragma once

#include "CoreMinimal.h"
#include "FrogAbilities/FrogAbility.h"
#include "FA_RoguePassive.generated.h"

UCLASS()
class FROG_API UFA_RoguePassive : public UFrogAbility
{
	GENERATED_BODY()

public:
	UFA_RoguePassive();
	
protected:
	FDelegateHandle EffectAppliedHandle;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	void OnGameplayEffectApplied(
		UAbilitySystemComponent* TargetASC,
		const FGameplayEffectSpec& SpecApplied,
		FActiveGameplayEffectHandle ActiveHandle
	);
};
