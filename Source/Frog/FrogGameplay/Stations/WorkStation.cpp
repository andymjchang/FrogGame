// Fill out your copyright notice in the Description page of Project Settings.

#include "WorkStation.h"

#include "Frog.h"
#include "FrogGameplay/Core/ContainerComponent.h"
#include "FrogGameplay/Core/ProgressTrackingComponent.h"

AWorkStation::AWorkStation()
{

}

void AWorkStation::StartWork(APlayerState* PlayerState)
{
    if (HasAuthority() && OfferedInteractable == this)
    {
        ContainerComponent->SetAllowRemove(false);
        ProgressTracker->StartProgressPassive();
    }
}

void AWorkStation::StopWork(APlayerState* PlayerState)
{
    if (HasAuthority())
    {
        ProgressTracker->StopProgressPassive();
    }
}

void AWorkStation::StopHighlight()
{
   
}