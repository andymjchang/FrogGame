#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "IngredientMap.h"
#include "FrogGameState.generated.h"

class AInteractable;

UENUM(BlueprintType)
enum class EFrogGamePhase : uint8
{
    Day,
    Night
};

// Delegates
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
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Frog Game|Data")
    UIngredientMap* IngredientMap;

    UFUNCTION(BlueprintCallable, Category = "Frog Game|Data")
    TSubclassOf<AInteractable> GetResultInteractableClass(const FGameplayTagContainer& InteractableTags) const
    {
        if (IngredientMap)
        {
            return IngredientMap->GetInteractableClassByBehavior(InteractableTags);
        }
        return nullptr;
    }

    // Replicated variables
    UPROPERTY(ReplicatedUsing = OnRep_Score, BlueprintReadOnly, Category = "Frog Game")
    int32 Score = 0;

    UPROPERTY(ReplicatedUsing = OnRep_CurrentPhase, BlueprintReadOnly, Category = "Frog Game")
    EFrogGamePhase CurrentPhase = EFrogGamePhase::Day;

    UPROPERTY(ReplicatedUsing = OnRep_PhaseEndTime, BlueprintReadOnly, Category = "Frog Game")
    float PhaseEndTime;

    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "Frog Game|Events")
    FOnScoreChanged OnScoreChanged;

    UPROPERTY(BlueprintAssignable, Category = "Frog Game|Events")
    FOnPhaseChanged OnPhaseChanged;

    UPROPERTY(BlueprintAssignable, Category = "Frog Game|Events")
    FOnPhaseEndTimeUpdated OnPhaseEndTimeUpdated;

    // Server functions
    UFUNCTION(BlueprintCallable, Category = "Frog Game")
    void TriggerDayPhase(float DayDuration);

    UFUNCTION(BlueprintCallable, Category = "Frog Game")
    void TriggerNightPhase();

    UFUNCTION(BlueprintCallable, Category = "Frog Game")
    void AddScore(int32 Amount);

    // Client functions
    UFUNCTION(BlueprintPure, Category = "Frog Game")
    float GetTimeRemaining() const;

protected:
    // OnRep functions
    UFUNCTION()
    void OnRep_Score();
    UFUNCTION()
    void OnRep_CurrentPhase();
    UFUNCTION()
    void OnRep_PhaseEndTime();
};