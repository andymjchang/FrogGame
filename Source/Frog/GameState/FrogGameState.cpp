// Fill out your copyright notice in the Description page of Project Settings.

#include "FrogGameState.h"

#include "FrogCharacter/FrogController.h"
#include "GameUI/FrogHUD.h"
#include "Net/UnrealNetwork.h"

AFrogGameState::AFrogGameState()
{
    PrimaryActorTick.bCanEverTick = true;
    Score = 0;
    PhaseEndTime = 0.0f;
    CurrentPhase = EFrogGamePhase::Night;
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

void AFrogGameState::UpdateScore()
{
    UWorld* World = GetWorld();
    if (!World) return;

    APlayerController* PlayerController = World->GetFirstPlayerController();

    // 4. Safety Check and Execute
    if (AFrogController* FrogPC = Cast<AFrogController>(PlayerController))
    {
        FrogPC->GetFrogHUD()->UpdateScoreText(Score);
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

void AFrogGameState::AddScore(int32 Amount)
{
    if (!HasAuthority()) return;

    Score += Amount;
    OnRep_Score();
}

float AFrogGameState::GetTimeRemaining() const
{
    if (PhaseEndTime <= 0.0f) return 0.0f;

    float Remaining = PhaseEndTime - GetServerWorldTimeSeconds();
    return FMath::Max(Remaining, 0.0f);
}

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

void AFrogGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AFrogGameState, Score);
    DOREPLIFETIME(AFrogGameState, CurrentPhase);
    DOREPLIFETIME(AFrogGameState, PhaseEndTime);
}