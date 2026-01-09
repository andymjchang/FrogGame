#include "FrogHUD.h"
#include "Kismet/GameplayStatics.h"

void UFrogHUD::NativeConstruct()
{
    Super::NativeConstruct();
    
    FrogGameState = Cast<AFrogGameState>(UGameplayStatics::GetGameState(GetWorld()));

    if (FrogGameState.IsValid())
    {
        OnScoreChanged(FrogGameState->GetScore());
        OnPhaseChanged(FrogGameState->GetCurrentPhase());
        
        FrogGameState->OnScoreChanged.AddUObject(this, &UFrogHUD::OnScoreChanged);
        FrogGameState->OnPhaseChanged.AddUObject(this, &UFrogHUD::OnPhaseChanged);
        
        if (GetWorld())
        {
            GetWorld()->GetTimerManager().SetTimer(
                TimerHandle_UIUpdate,
                this,
                &UFrogHUD::UpdateCountdownDisplay,
                1.0f, // Refresh rate
                true  // Loop
            );
        }
    }
}

void UFrogHUD::NativeDestruct()
{
    Super::NativeDestruct();

    // Clean up timer
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle_UIUpdate);
    }

    // Clean up delegates
    if (FrogGameState.IsValid())
    {
        FrogGameState->OnScoreChanged.RemoveAll(this);
        FrogGameState->OnPhaseChanged.RemoveAll(this);
    }
}

void UFrogHUD::OnScoreChanged(int32 NewScore)
{
    if (!IsValid(ScoreText)) return;
    
    ScoreText->SetText(FText::AsNumber(NewScore));
    
}

void UFrogHUD::OnPhaseChanged(EFrogGamePhase NewPhase)
{
    if (!IsValid(DayTimerText)) return;
    
    if (NewPhase == EFrogGamePhase::Night)
    {
        DayTimerText->SetText(FText::FromString("Night"));
        DayTimerText->SetColorAndOpacity(FLinearColor::Gray);
    }
    else if (NewPhase == EFrogGamePhase::Day)
    {
        DayTimerText->SetColorAndOpacity(FLinearColor::White);
        UpdateCountdownDisplay();
    }
    
}

void UFrogHUD::UpdateCountdownDisplay()
{
    if (!FrogGameState.IsValid() || !IsValid(DayTimerText)) return;

    if (FrogGameState->GetCurrentPhase() == EFrogGamePhase::Day)
    {
        const float TimeRemaining = FrogGameState->GetTimeRemaining();
        const int32 TotalSeconds = FMath::Max(0, FMath::CeilToInt(TimeRemaining));
        const int32 Minutes = TotalSeconds / 60;
        const int32 Seconds = TotalSeconds % 60;

        FString TimeString = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
        DayTimerText->SetText(FText::FromString(TimeString));
    }
}