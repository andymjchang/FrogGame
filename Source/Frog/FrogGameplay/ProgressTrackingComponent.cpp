// Fill out your copyright notice in the Description page of Project Settings.


#include "ProgressTrackingComponent.h"

#include "GameUI/Interactables/StationProgressBar.h"


UProgressTrackingComponent::UProgressTrackingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	PlayerContributions.Reserve(NUM_PLAYERS);
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
		PassiveProgress += DeltaTime;
		const float ProgressPercent = GetProgressFraction();
		
		if (ProgressBarWidget.IsValid())
		{
			ProgressBarWidget->SetProgressPercent(ProgressPercent);
		}
		
		if (ProgressPercent >= 1.0f)
		{
			CompleteProgress();
		}
	}
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
	bIsProcessing = true;
	SetWidgetVisibility(true);
}

void UProgressTrackingComponent::AddProgress()
{
	
}

void UProgressTrackingComponent::StopProgress()
{
	bIsProcessing = false;
	if (GetProgressFraction() <= 0.0f)
	{
		SetWidgetVisibility(false);
	}
}

void UProgressTrackingComponent::CompleteProgress()
{
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

