// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "FrogHUD.generated.h"



/**
 * */
UCLASS()
class FROG_API UFrogHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ScoreText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DayTimerText;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateScoreText(int NewScore) const;
};