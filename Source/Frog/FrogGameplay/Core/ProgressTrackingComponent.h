// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProgressTrackingComponent.generated.h"

class UStationProgressBar;
class UInteractableWidgetComponent;

static constexpr int32 NUM_PLAYERS = 4;
static constexpr int32 SHARED_PROGRESS_INDEX = 0;
DECLARE_DYNAMIC_DELEGATE(FOnCompletion);

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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetProgressWidgetReference(UUserWidget* InProgressBarWidget);
	void StartProgressPassive();
	void StopProgressPassive();
	void ResetProgress();
	
	void SetProgressDuration(float InDuration);

	void AddProgressFlat();
	void AddProgressPercentage(float Percentage, const APlayerState* PlayerState);
	
	// Delegates
	FOnCompletion OnCompletion;
	
protected: // Protected Functions
	virtual void BeginPlay() override;
	
	float GetProgressFraction() const;
	void CompleteProgress();
	
	void SetWidgetVisibility(bool IsVisible);
	void SetWidgetProgress();

	// On Rep Functions
	UFUNCTION()
	void OnRep_IsProcessing();
	
	UFUNCTION()
	void OnRep_ProgressPerPlayerSeconds();

	UFUNCTION()
	void OnRep_SharedProgressSeconds();

protected: // Protected Members
	// UPROPERTY(ReplicatedUsing = OnRep_IsProcessing)
	bool bIsProcessing;

	UPROPERTY(ReplicatedUsing = OnRep_ProgressPerPlayerSeconds)
	float ProgressPerPlayerSeconds[NUM_PLAYERS];

	UPROPERTY(ReplicatedUsing = OnRep_SharedProgressSeconds)
	float SharedProgressSeconds;
	
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	EProgressMethod ProgressMethod = EProgressMethod::Passive;

	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	EProgressScope ProgressScope = EProgressScope::Shared;
	
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	bool bIsDecayEnabled;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "User Settings")
	float TargetDuration = 1.0f;	

	// UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	// float DecayRate = 0.0f;	
	
protected: // Protected Components
	TWeakObjectPtr<UStationProgressBar> ProgressBarWidget;

};
