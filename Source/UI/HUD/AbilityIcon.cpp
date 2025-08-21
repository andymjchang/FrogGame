// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityIcon.h"
#include "AbilitySystemComponent.h"
#include "Components/ProgressBar.h"

void UAbilityIcon::TrackAbility(UAbilitySystemComponent* InputAbilitySystem, UGameplayAbility* InputAbility)
{
	if (InputAbilitySystem && InputAbility)
	{
		TrackedAbilitySystem = InputAbilitySystem;
		TrackedAbility = InputAbility;
	}
}

void UAbilityIcon::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!TrackedAbilitySystem || !TrackedAbility.IsValid()) return;
	
	float TimeRemaining = 0.0f;
	float CooldownDuration = 0.0f;
	
	TrackedAbility->GetCooldownTimeRemainingAndDuration(
		TrackedAbility->GetCurrentAbilitySpecHandle(),
		TrackedAbilitySystem->AbilityActorInfo.Get(),
		TimeRemaining,
		CooldownDuration
	);

	ProgressBar->SetPercent(TimeRemaining / CooldownDuration);
}
