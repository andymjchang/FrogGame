#include "FrogHUD.h"
#include "Kismet/GameplayStatics.h"

void UFrogHUD::NativeConstruct()
{
    Super::NativeConstruct();

    // 1. Get the GameState and cast it
    AGameStateBase* GenericGS = UGameplayStatics::GetGameState(GetWorld());
    FrogGameState = Cast<AFrogGameState>(GenericGS);

    if (FrogGameState.IsValid())
    {
        // 2. Initial Setup: Update UI with current values immediately
        OnScoreChanged(FrogGameState->Score);
        OnPhaseChanged(FrogGameState->CurrentPhase);

        // 3. Bind to Delegates so we react to changes automatically
        FrogGameState->OnScoreChanged.AddDynamic(this, &UFrogHUD::OnScoreChanged);
        FrogGameState->OnPhaseChanged.AddDynamic(this, &UFrogHUD::OnPhaseChanged);

        // 4. Start a loop to update the timer text
        // We update every 0.1s to ensure the seconds flip smoothly on screen.
        if (GetWorld())
        {
            GetWorld()->GetTimerManager().SetTimer(
                TimerHandle_UIUpdate,
                this,
                &UFrogHUD::UpdateCountdownDisplay,
                0.1f, // Refresh rate
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
        FrogGameState->OnScoreChanged.RemoveDynamic(this, &UFrogHUD::OnScoreChanged);
        FrogGameState->OnPhaseChanged.RemoveDynamic(this, &UFrogHUD::OnPhaseChanged);
    }
}

void UFrogHUD::OnScoreChanged(int32 NewScore)
{
    if (ScoreText)
    {
        ScoreText->SetText(FText::AsNumber(NewScore));
    }
}

void UFrogHUD::OnPhaseChanged(EFrogGamePhase NewPhase)
{
    // Optional: Hide timer if it is Night, or change color
    if (DayTimerText)
    {
        if (NewPhase == EFrogGamePhase::Night)
        {
            DayTimerText->SetText(FText::FromString("Night"));
            DayTimerText->SetColorAndOpacity(FLinearColor::Gray);
        }
        else
        {
            DayTimerText->SetColorAndOpacity(FLinearColor::White);
            // Force an immediate update
            UpdateCountdownDisplay();
        }
    }
}

void UFrogHUD::UpdateCountdownDisplay()
{
    if (!FrogGameState.IsValid() || !DayTimerText) return;

    // Only update the numbers if we are in the Day phase
    if (FrogGameState->CurrentPhase == EFrogGamePhase::Day)
    {
        // 1. Get the authoritative time from GameState
        float TimeRemaining = FrogGameState->GetTimeRemaining();

        // 2. Math: CeilToInt makes 0.9s show as "1" and 0.0s show as "0"
        int32 TotalSeconds = FMath::CeilToInt(TimeRemaining);
        
        // Clamp to 0 to prevent negative numbers just in case
        TotalSeconds = FMath::Max(0, TotalSeconds);

        const int32 Minutes = TotalSeconds / 60;
        const int32 Seconds = TotalSeconds % 60;

        // 3. Format MM:SS
        FString TimeString = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
        DayTimerText->SetText(FText::FromString(TimeString));
    }
}