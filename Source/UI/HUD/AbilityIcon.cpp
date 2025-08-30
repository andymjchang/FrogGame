// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityIcon.h"
#include "AbilitySystemComponent.h"
#include "Components/ProgressBar.h"

void UAbilityIcon::TrackAbility(UAbilitySystemComponent* InputAbilitySystem, const FGameplayAbilitySpecHandle InputAbilityHandle)
{
	if (InputAbilitySystem)
	{
		TrackedAbilitySystem = InputAbilitySystem;
		TrackedAbilityHandle = InputAbilityHandle;
	}
}

// void UAbilityIcon::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
// {
// 	Super::NativeTick(MyGeometry, InDeltaTime);
//
// 	if (!TrackedAbilitySystem.IsValid() || !TrackedAbility.IsValid()) return;
// 	
// 	float TimeRemaining = 0.0f;
// 	float CooldownDuration = 0.0f;
// 	
// 	TrackedAbility->GetCooldownTimeRemainingAndDuration(
// 		TrackedAbility->GetCurrentAbilitySpecHandle(),
// 		TrackedAbilitySystem->AbilityActorInfo.Get(),
// 		TimeRemaining,
// 		CooldownDuration
// 	);
//
// 	ProgressBar->SetPercent(TimeRemaining / CooldownDuration);
// }

void UAbilityIcon::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (!TrackedAbilitySystem.IsValid() || !TrackedAbilityHandle.IsValid())
	{
		if (ProgressBar)
		{
			ProgressBar->SetPercent(0.0f);
		}
		return;
	}
    
	float TimeRemaining = 0.0f;
	float CooldownDuration = 0.0f;
    
	FGameplayAbilitySpec* AbilitySpec = TrackedAbilitySystem->FindAbilitySpecFromHandle(TrackedAbilityHandle);
	if (AbilitySpec)
	{
		if (UGameplayAbility* AbilityInstance = AbilitySpec->GetPrimaryInstance())
		{
			AbilityInstance->GetCooldownTimeRemainingAndDuration(
			   TrackedAbilityHandle,
			   AbilityInstance->GetCurrentActorInfo(),
			   TimeRemaining,
			   CooldownDuration
				);
		}
	}
	

	if (ProgressBar)
	{
		if (CooldownDuration > 0.0f)
		{
			ProgressBar->SetPercent(TimeRemaining / CooldownDuration);
		}
		else
		{
			ProgressBar->SetPercent(0.0f);
		}
	}
}