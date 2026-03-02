// Fill out your copyright notice in the Description page of Project Settings.

#include "WorkStation.h"

#include "ProgressTrackingComponent.h"
#include "Components/BoxComponent.h"

AWorkStation::AWorkStation()
{

}

void AWorkStation::StartWork()
{
    if (HasAuthority())
    {
        ProgressTracker->StartProgress();
    }
}

void AWorkStation::StopWork()
{
    if (HasAuthority())
    {
        ProgressTracker->StopProgress();
    }
}

void AWorkStation::StopHighlight()
{
    Super::StopHighlight();
    
    if (HasAuthority())
    {
        ProgressTracker->StopProgress();
    }
}