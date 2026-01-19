// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoStation.h"
#include "GameUI/Interactables/InteractableWidgetComponent.h"

// Sets default values
AAutoStation::AAutoStation()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAutoStation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAutoStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAutoStation::HandleInteractableAdded(AInteractable* InteractableToAdd)
{
	UE_LOG(LogTemp, Log, TEXT("[%f] _____AutoStation_____"), GetWorld()->GetTimeSeconds());

	// Don't autostart if the added item is the finished item
	if (OfferedInteractable == this)
	{
		bIsProcessing = true;
		ProcessStartTime = GetWorld()->GetTimeSeconds();
		ProcessEndTime = ProcessStartTime + ProcessingDuration;

		if (ProgressBarWidgetComponent)
		{
			ProgressBarWidgetComponent->SetVisibility(true);
		}
	}
}

