// Fill out your copyright notice in the Description page of Project Settings.


#include "ProgressTrackingComponent.h"

#include "Frog.h"
#include "GameUI/Interactables/StationProgressBar.h"
#include "Net/UnrealNetwork.h"


UProgressTrackingComponent::UProgressTrackingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	
	PlayerContributions.Reserve(NUM_PLAYERS);
}


void UProgressTrackingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UProgressTrackingComponent, bIsProcessing);
	DOREPLIFETIME(UProgressTrackingComponent, StartTime);
}

void UProgressTrackingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UProgressTrackingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ProgressMethod == EProgressMethod::Passive && bIsProcessing)
	{
		if (GetOwner()->HasAuthority())
		{
			PassiveProgress += DeltaTime;
			if (GetProgressFraction() >= 1.0f)
			{
				CompleteProgress();
			}
		}
		
		// Update UI on both server and client
		const float ProgressPercent = GetProgressFraction();
		if (ProgressBarWidget.IsValid())
		{
			ProgressBarWidget->SetProgressPercent(ProgressPercent);
		}
	}
}

void UProgressTrackingComponent::OnRep_IsProcessing()
{
	SetWidgetVisibility(bIsProcessing || GetProgressFraction() > 0.0f);
}

void UProgressTrackingComponent::SetProgressWidget(UUserWidget* InProgressBarWidget)
{
	if (UStationProgressBar* ProgressWidget = Cast<UStationProgressBar>(InProgressBarWidget))
	{
		ProgressBarWidget = ProgressWidget;
		SetWidgetVisibility(false);
	}
}

void UProgressTrackingComponent::SetProgressDuration(const float InDuration)
{
	TargetDuration = InDuration;
}

void UProgressTrackingComponent::SetProgressStartTime(const float InStartTime)
{
	StartTime = InStartTime;
}

float UProgressTrackingComponent::GetProgressFraction() const
{
	if (ProgressMethod == EProgressMethod::Passive && bIsProcessing && !GetOwner()->HasAuthority())
	{
		// Client estimation
		float Elapsed = GetWorld()->GetTimeSeconds() - StartTime;
		return FMath::Clamp(Elapsed / TargetDuration, 0.0f, 1.0f);
	}

	float CurrentProgress = PassiveProgress;
	if (ProgressScope == EProgressScope::Individual)
	{
		for (const float Progress : PlayerContributions)
		{
			CurrentProgress += Progress;
		}
	}

	return FMath::Clamp(CurrentProgress / TargetDuration, 0.0f, 1.0f);
}

void UProgressTrackingComponent::StartProgress()
{
	if (!GetOwner()->HasAuthority()) return;

	bIsProcessing = true;
	StartTime = GetWorld()->GetTimeSeconds();
	SetWidgetVisibility(true);
}

void UProgressTrackingComponent::AddProgress()
{
	
}

void UProgressTrackingComponent::AddProgressByPercentage(float pct)
{
	if (!GetOwner()->HasAuthority()) return;
	if (ProgressMethod != EProgressMethod::Active) return;

	const float ProgressToAdd = pct * TargetDuration/100.0f;
	PassiveProgress += ProgressToAdd;

	const float ProgressPercent = GetProgressFraction();

	if (ProgressBarWidget.IsValid())
	{
		ProgressBarWidget->SetProgressPercent(ProgressPercent);
	}

	if (ProgressPercent >= 1.0f)
	{
		CompleteProgress();
	}else
	{
		SetWidgetVisibility(true);
	}
}

void UProgressTrackingComponent::ResetProgress()
{
	if (!GetOwner()->HasAuthority()) return;
	
	PassiveProgress = 0.0f;
	bIsProcessing = false;
	
	SetWidgetVisibility(false);
}

void UProgressTrackingComponent::StopProgress()
{
	if (!GetOwner()->HasAuthority()) return;

	bIsProcessing = false;
	if (GetProgressFraction() <= 0.0f)
	{
		SetWidgetVisibility(false);
	}
}

void UProgressTrackingComponent::CompleteProgress()
{
	if (!GetOwner()->HasAuthority()) return;

	bIsProcessing = false;
	SetWidgetVisibility(false);
	
	PassiveProgress = 0.0f;
	if (ProgressScope == EProgressScope::Individual)
	{
		for (float& Val : PlayerContributions)
		{
			Val = 0.0f;
		}
	}
	
	OnCompletion.ExecuteIfBound();
}

void UProgressTrackingComponent::SetWidgetVisibility(const bool Value)
{
	if (!ProgressBarWidget.IsValid()) return;
	
	if (Value == true)
	{
		ProgressBarWidget->SetVisibility(ESlateVisibility::Visible);	
	} 
	else
	{
		ProgressBarWidget->SetVisibility(ESlateVisibility::Hidden);
	} 
}

