#pragma once

#include "CoreMinimal.h"
#include "FrogAbilities/FrogGameplayAbility.h"
#include "FrogRoguePassive.generated.h"

UCLASS()
class FROG_API UFrogRoguePassive : public UFrogGameplayAbility
{
	GENERATED_BODY()

public:
	UFrogRoguePassive();
	
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
