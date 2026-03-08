// Fill out your copyright notice in the Description page of Project Settings.


#include "ProgressTrackingComponent.h"

#include "GameFramework/GameStateBase.h"
#include "GameUI/Interactables/StationProgressBar.h"
#include "Net/UnrealNetwork.h"


UProgressTrackingComponent::UProgressTrackingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.2f;
	SetIsReplicatedByDefault(true);
	
	// PlayerContributions.Reserve(NUM_PLAYERS);
	PlayerContributions.Init(0.f, NUM_PLAYERS);
}


void UProgressTrackingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UProgressTrackingComponent, bIsProcessing);
	DOREPLIFETIME(UProgressTrackingComponent, StartTime);
	DOREPLIFETIME(UProgressTrackingComponent, TargetDuration);
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

void UProgressTrackingComponent::SetProgressWidgetReference(UUserWidget* InProgressBarWidget)
{
	if (UStationProgressBar* ProgressWidget = Cast<UStationProgressBar>(InProgressBarWidget))
	{
		ProgressBarWidget = ProgressWidget;
		SetWidgetVisibility(false);
		ProgressBarWidget->SetProgressPercent(0.f);
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
		if (AGameStateBase* GameState = GetWorld()->GetGameState())
		{
			const float ServerTime = GameState->GetServerWorldTimeSeconds();
			const float ElapsedTime = ServerTime - StartTime;
			return FMath::Clamp(ElapsedTime / TargetDuration, 0.0f, 1.0f);
		}
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
	OnRep_IsProcessing();
	
	if (AGameStateBase* GameState = GetWorld()->GetGameState())
	{
		const float ServerTime = GameState->GetServerWorldTimeSeconds();
		StartTime = ServerTime;
	}
}

void UProgressTrackingComponent::AddProgress()
{
	
}

// TODO: Change to flat amount if process time is based on number / type of ingredients
void UProgressTrackingComponent::AddProgressByPercentage(float Percentage)
{
	if (!GetOwner()->HasAuthority()) return;
	if (ProgressMethod != EProgressMethod::Active) return;

	const float ProgressToAdd = Percentage * TargetDuration / 100.0f;
	PassiveProgress += ProgressToAdd;

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

void UProgressTrackingComponent::ResetProgress()
{
	if (!GetOwner()->HasAuthority()) return;
	
	PassiveProgress = 0.0f;
	bIsProcessing = false;
	OnRep_IsProcessing();
}

void UProgressTrackingComponent::StopProgress()
{
	if (!GetOwner()->HasAuthority()) return;

	bIsProcessing = false;
	OnRep_IsProcessing();
}

void UProgressTrackingComponent::CompleteProgress()
{
	if (!GetOwner()->HasAuthority()) return;
	
	PassiveProgress = 0.0f;
	if (ProgressScope == EProgressScope::Individual)
	{
		for (float& Val : PlayerContributions)
		{
			Val = 0.0f;
		}
	}

	bIsProcessing = false;
	OnRep_IsProcessing();
	
	OnCompletion.ExecuteIfBound();
}

void UProgressTrackingComponent::SetWidgetVisibility(const bool IsVisible)
{
	if (!ProgressBarWidget.IsValid()) return;
	
	if (IsVisible == true)
	{
		ProgressBarWidget->SetVisibility(ESlateVisibility::Visible);	
	} 
	else
	{
		ProgressBarWidget->SetVisibility(ESlateVisibility::Hidden);
	} 
}

