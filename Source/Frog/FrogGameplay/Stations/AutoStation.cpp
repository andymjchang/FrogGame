// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoStation.h"

#include "Frog.h"
#include "FrogGameplay/Core/ContainerComponent.h"
#include "FrogGameplay/Core/ProgressTrackingComponent.h"

// Sets default values
AAutoStation::AAutoStation()
{
	PrimaryActorTick.bCanEverTick = false;
}

// TODO: Either make processing duration dynamic based on ingredients / inventory size
// or set duration in CDO 
void AAutoStation::HandleAddedToInventory(const TScriptInterface<IItemInterface>& InteractableToAdd)
{
	// Don't autostart if the added item is the finished item
	if (OfferedInteractable == this)
	{
		ContainerComponent->SetAllowRemove(false);
		FLOG(TEXT("asd;flkj"))
		ProgressTracker->StartProgressPassive();
	}
}

