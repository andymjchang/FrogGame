// Fill out your copyright notice in the Description page of Project Settings.


#include "PressStation.h"

#include "FrogGameplay/Core/ProgressTrackingComponent.h"


APressStation::APressStation()
{

}

void APressStation::StartWork()
{
	//FLOG(TEXT("APressStation::StartWork - Adding %.2f%% progress. Current progress: %.2f%%"), 10.0, ProgressTracker->GetProgressFraction() * 100.0f);
	if (HasAuthority())
	{
		ProgressTracker->AddProgressByPercentage(PctAddedPerPress);
	}
}
