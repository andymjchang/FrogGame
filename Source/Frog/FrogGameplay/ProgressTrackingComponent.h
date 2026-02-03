// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProgressTrackingComponent.generated.h"

class UStationProgressBar;
class UInteractableWidgetComponent;

static constexpr int NUM_PLAYERS = 4;
DECLARE_MULTICAST_DELEGATE(FOnCompletion);

UENUM(BlueprintType)
enum class EProgressMethod : uint8
{
	Passive     UMETA(DisplayName = "Passive (Over Time)"),
	Active      UMETA(DisplayName = "Active (Input Based)")
};

UENUM(BlueprintType)
enum class EProgressScope : uint8
{
	Shared      UMETA(DisplayName = "Shared (Global)"),
	Individual  UMETA(DisplayName = "Individual (All Must Complete)")
};

UCLASS()
class FROG_API UProgressTrackingComponent : public UActorComponent
{
	GENERATED_BODY()

public: // Public Functions
	UProgressTrackingComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void SetProgressWidget(UStationProgressBar* InProgressBarWidget);
	
	void SetProgressDuration(float InDuration);
	void SetProgressStartTime(float InStartTime);
	
	float GetProgressFraction() const;
	
	void StartProgress();
	void AddProgress();
	void StopProgress();

public: // Public Members
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	EProgressMethod ProgressMethod = EProgressMethod::Passive;

	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	EProgressScope ProgressScope = EProgressScope::Shared;
	
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	bool bIsDecayEnabled;
	
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	float TargetDuration = 1.0f;
	
	// Delegates
	FOnCompletion OnCompletion;

	// UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	// float DecayRate = 0.0f;
	
protected: // Protected Functions
	virtual void BeginPlay() override;
	void CompleteProgress();
	void SetWidgetVisibility(bool Value);

protected: // Protected Members
	bool bIsProcessing;
	float StartTime;
	TArray<float> PlayerContributions;
	float PassiveProgress;
	
protected: // Protected Components
	TSoftObjectPtr<UStationProgressBar> ProgressBarWidget;

};
