// Fill out your copyright notice in the Description page of Project Settings.


#include "ProgressTrackingComponent.h"

#include "Frog.h"
#include "GameFramework/GameStateBase.h"
#include "GameState/FrogGameState.h"
#include "GameUI/Interactables/StationProgressBar.h"
#include "Net/UnrealNetwork.h"


UProgressTrackingComponent::UProgressTrackingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1f;
	SetIsReplicatedByDefault(true);
	
	for (int32 i = 0; i < NUM_PLAYERS; ++i)
	{
		ProgressPerPlayerSeconds[i] = 0.f;
	}
	SharedProgressSeconds = 0.f;
}


void UProgressTrackingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(UProgressTrackingComponent, bIsProcessing);
	DOREPLIFETIME(UProgressTrackingComponent, TargetDuration);
	DOREPLIFETIME(UProgressTrackingComponent, ProgressPerPlayerSeconds);
	DOREPLIFETIME(UProgressTrackingComponent, SharedProgressSeconds);
}

void UProgressTrackingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UProgressTrackingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const AActor* Owner = GetOwner();
	if (!IsValid(Owner)) return;

	if (Owner->HasAuthority() && bIsProcessing && ProgressMethod == EProgressMethod::Passive)
	{
		SharedProgressSeconds += DeltaTime;
		OnRep_SharedProgressSeconds();
		if (GetProgressFraction() >= 1.0f)
		{
			CompleteProgress();
		}
	}
}

void UProgressTrackingComponent::OnRep_IsProcessing()
{
	SetWidgetProgress();
}

void UProgressTrackingComponent::OnRep_ProgressPerPlayerSeconds()
{
	SetWidgetProgress();
}

void UProgressTrackingComponent::OnRep_SharedProgressSeconds()
{
	SetWidgetProgress();
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

float UProgressTrackingComponent::GetProgressFraction() const
{
	// if (ProgressMethod == EProgressMethod::Passive && bIsProcessing && !GetOwner()->HasAuthority())
	// {
	// 	// Client estimation
	// 	if (AGameStateBase* GameState = GetWorld()->GetGameState())
	// 	{
	// 		const float ServerTime = GameState->GetServerWorldTimeSeconds();
	// 		const float ElapsedTime = ServerTime - StartTime;
	// 		return FMath::Clamp(ElapsedTime / TargetDuration, 0.0f, 1.0f);
	// 	}
	// }

	float CurrentProgress = 0;
	if (ProgressScope == EProgressScope::Individual)
	{
		for (int32 i = 0; i < NUM_PLAYERS; ++i)
		{
			CurrentProgress += ProgressPerPlayerSeconds[i];
		}
	}
	else
	{
		CurrentProgress = SharedProgressSeconds;
	}

	return FMath::Clamp(CurrentProgress / TargetDuration, 0.0f, 1.0f);
}

void UProgressTrackingComponent::StartProgressPassive()
{
	if (!GetOwner()->HasAuthority()) return;

	bIsProcessing = true;
	// OnRep_IsProcessing();
}

void UProgressTrackingComponent::AddProgressFlat()
{
	
}

// TODO: Change to flat amount if process time is based on number / type of ingredients
void UProgressTrackingComponent::AddProgressPercentage(float Percentage, const APlayerState* PlayerState)
{
	if (!GetOwner()->HasAuthority()) return;

	const float ProgressToAdd = Percentage * TargetDuration / 100.0f;
	
	if (ProgressScope == EProgressScope::Individual)
	{
		if (AFrogGameState* GameState = Cast<AFrogGameState>(GetWorld()->GetGameState()))
		{
			const int32 PlayerIndex = GameState->GetPlayerIndex(PlayerState);
			if (PlayerIndex >= 0 && PlayerIndex < NUM_PLAYERS)
			{
				ProgressPerPlayerSeconds[PlayerIndex] += ProgressToAdd;
			}
		}
		OnRep_ProgressPerPlayerSeconds();
	}
	
	if (ProgressScope == EProgressScope::Shared)
	{
		SharedProgressSeconds += ProgressToAdd;
		OnRep_SharedProgressSeconds();
	}

	if (GetProgressFraction() >= 1.0f)
	{
		CompleteProgress();
	}
}

void UProgressTrackingComponent::ResetProgress()
{
	if (!GetOwner()->HasAuthority()) return;
	
	for (int32 i = 0; i < NUM_PLAYERS; ++i)
	{
		ProgressPerPlayerSeconds[i] = 0.f;
	}
	SharedProgressSeconds = 0.f;
	OnRep_ProgressPerPlayerSeconds();
	OnRep_SharedProgressSeconds();
	
	bIsProcessing = false;
	// OnRep_IsProcessing();
}

// TODO: track all player's working status independently so it doesn't stop a player is still processing while another stops
void UProgressTrackingComponent::StopProgressPassive()
{
	if (!GetOwner()->HasAuthority()) return;

	bIsProcessing = false;
	// OnRep_IsProcessing();
}

void UProgressTrackingComponent::CompleteProgress()
{
	if (!GetOwner()->HasAuthority()) return;
	
	ResetProgress();		
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

void UProgressTrackingComponent::SetWidgetProgress()
{
	SetWidgetVisibility(GetProgressFraction() > 0.0f);
	if (ProgressBarWidget.IsValid())
	{
		ProgressBarWidget->SetProgressPercent(GetProgressFraction());
	}	
}

