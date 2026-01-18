#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Components/TextBlock.h"
#include "GameState/FrogGameState.h" 
#include "FrogHUD.generated.h"

UCLASS()
class FROG_API UFrogHUD : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	// Widget objects
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ScoreText;
    
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DayTimerText;
	
	TWeakObjectPtr<AFrogGameState> FrogGameState;
	
	FTimerHandle TimerHandle_UIUpdate;

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	// Delegate functions
	UFUNCTION()
	void OnScoreChanged(int32 NewScore);
	UFUNCTION()
	void OnPhaseChanged(EFrogGamePhase NewPhase);

	void UpdateCountdownDisplay();
};