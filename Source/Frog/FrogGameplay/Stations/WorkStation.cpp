// Fill out your copyright notice in the Description page of Project Settings.

#include "WorkStation.h"

#include "Components/BoxComponent.h"
#include "FrogGameplay/Core/ProgressTrackingComponent.h"

AWorkStation::AWorkStation()
{

}

void AWorkStation::StartWork()
{
    if (HasAuthority() && OfferedInteractable == this)
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
}