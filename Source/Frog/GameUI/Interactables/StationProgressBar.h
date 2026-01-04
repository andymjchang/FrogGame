// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "StationProgressBar.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class FROG_API UStationProgressBar : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	void SetPercentProgress(float NewPercentage);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar;
	
};
