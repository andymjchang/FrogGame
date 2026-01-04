// Fill out your copyright notice in the Description page of Project Settings.


#include "StationProgressBar.h"

#include "Components/ProgressBar.h"

void UStationProgressBar::SetPercentProgress(const float NewPercentage)
{
	ProgressBar->SetPercent(NewPercentage);
}
