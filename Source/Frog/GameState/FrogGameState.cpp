#include "FrogGameState.h"
#include "Net/UnrealNetwork.h"

AFrogGameState::AFrogGameState()
{
    PrimaryActorTick.bCanEverTick = true;
    
    // Initialize Defaults
    Score = 0;
    PhaseEndTime = 0.0f;
    CurrentPhase = EFrogGamePhase::Night;
}

void AFrogGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AFrogGameState, Score);
    DOREPLIFETIME(AFrogGameState, CurrentPhase);
    DOREPLIFETIME(AFrogGameState, PhaseEndTime);
}

TSubclassOf<AInteractable> AFrogGameState::GetResultInteractableClass(
    const FGameplayTagContainer& InteractableTags) const
{
    {
        if (IngredientMap)
        {
            return IngredientMap->GetInteractableClassByBehavior(InteractableTags);
        }
        return nullptr;
    }
}

void AFrogGameState::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    
    if (!HasAuthority()) return;
    
    if (CurrentPhase == EFrogGamePhase::Day)
    {
        if (GetServerWorldTimeSeconds() >= PhaseEndTime)
        {
            TriggerNightPhase();
        }
    }
}

void AFrogGameState::TriggerDayPhase(float DayDuration)
{
    if (!HasAuthority()) return;

    CurrentPhase = EFrogGamePhase::Day;
    PhaseEndTime = GetServerWorldTimeSeconds() + DayDuration;
    
    OnRep_CurrentPhase();
    OnRep_PhaseEndTime();
}

void AFrogGameState::TriggerNightPhase()
{
    if (!HasAuthority()) return;

    CurrentPhase = EFrogGamePhase::Night;
    PhaseEndTime = 0.0f;
    
    OnRep_CurrentPhase();
    OnRep_PhaseEndTime();
}

void AFrogGameState::ServerAddScore(int32 Amount)
{
    if (!HasAuthority()) return;

    Score += Amount;
    OnRep_Score();
}

float AFrogGameState::GetTimeRemaining() const
{
    if (CurrentPhase != EFrogGamePhase::Day || PhaseEndTime <= 0.0f) return 0.0f;
    return FMath::Max(PhaseEndTime - GetServerWorldTimeSeconds(), 0.0f);
}

// OnRep Functions
void AFrogGameState::OnRep_Score()
{
    OnScoreChanged.Broadcast(Score);
}

void AFrogGameState::OnRep_CurrentPhase()
{
    OnPhaseChanged.Broadcast(CurrentPhase);
}

void AFrogGameState::OnRep_PhaseEndTime()
{
    OnPhaseEndTimeUpdated.Broadcast(PhaseEndTime);
}