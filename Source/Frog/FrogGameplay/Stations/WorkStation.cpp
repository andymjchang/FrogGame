// Fill out your copyright notice in the Description page of Project Settings.

#include "WorkStation.h"

#include "Frog.h"
#include "Components/BoxComponent.h"
#include "FrogGameplay/Core/ProgressTrackingComponent.h"

AWorkStation::AWorkStation()
{

}

void AWorkStation::StartWork(APlayerState* PlayerState)
{
    if (HasAuthority() && OfferedInteractable == this)
    {
        ProgressTracker->StartProgressPassive();
    }
}

void AWorkStation::StopWork(APlayerState* PlayerState)
{
    if (HasAuthority())
    {
        FLOG(TEXT("stop"));
        ProgressTracker->StopProgressPassive();
    }
}

void AWorkStation::StopHighlight()
{
   
}