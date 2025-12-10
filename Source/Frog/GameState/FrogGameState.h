#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FrogGameState.generated.h"

UENUM(BlueprintType)
enum class EFrogGamePhase : uint8
{
	Day,
	Night
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, NewScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseChanged, EFrogGamePhase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseEndTimeUpdated, float, NewEndTime);

UCLASS()
class FROG_API AFrogGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AFrogGameState();

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(ReplicatedUsing = OnRep_Score, EditAnywhere, BlueprintReadWrite, Category = "Frog Game")
	int32 Score;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentPhase, EditAnywhere, BlueprintReadWrite, Category = "Frog Game")
	EFrogGamePhase CurrentPhase;

	UPROPERTY(ReplicatedUsing = OnRep_PhaseEndTime, EditAnywhere, BlueprintReadWrite, Category = "Frog Game")
	float PhaseEndTime;

	UPROPERTY(BlueprintAssignable, Category = "Frog Game")
	FOnScoreChanged OnScoreChanged;

	UPROPERTY(BlueprintAssignable, Category = "Frog Game")
	FOnPhaseChanged OnPhaseChanged;

	UPROPERTY(BlueprintAssignable, Category = "Frog Game")
	FOnPhaseEndTimeUpdated OnPhaseEndTimeUpdated;

	UFUNCTION(BlueprintCallable, Category = "Frog Game")
	void TriggerDayPhase(float DayDuration);

	UFUNCTION(BlueprintCallable, Category = "Frog Game")
	void TriggerNightPhase();

	UFUNCTION(BlueprintCallable, Category = "Frog Game")
	void AddScore(int32 Amount);

	UFUNCTION(BlueprintPure, Category = "Frog Game")
	float GetTimeRemaining() const;

protected:
	UFUNCTION()
	void OnRep_Score();

	UFUNCTION()
	void OnRep_CurrentPhase();

	UFUNCTION()
	void OnRep_PhaseEndTime();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};