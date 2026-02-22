// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoStation.h"

#include "ProgressTrackingComponent.h"

// Sets default values
AAutoStation::AAutoStation()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAutoStation::HandleAddedToInventory(AItem* InteractableToAdd)
{
	UE_LOG(LogTemp, Log, TEXT("[%f] _____AutoStation_____"), GetWorld()->GetTimeSeconds());

	// Don't autostart if the added item is the finished item
	if (OfferedInteractable == this)
	{
		ProgressTracker->SetProgressStartTime(GetWorld()->GetTimeSeconds());
		ProgressTracker->SetProgressDuration(ProcessingDuration);
		ProgressTracker->StartProgress();
	}
}

