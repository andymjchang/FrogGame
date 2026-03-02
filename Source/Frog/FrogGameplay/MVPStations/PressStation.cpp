// Fill out your copyright notice in the Description page of Project Settings.


#include "PressStation.h"

#include "Frog.h"
#include "FrogGameplay/ProgressTrackingComponent.h"


// Sets default values
APressStation::APressStation()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APressStation::StartWork()
{
	//FLOG(TEXT("APressStation::StartWork - Adding %.2f%% progress. Current progress: %.2f%%"), 10.0, ProgressTracker->GetProgressFraction() * 100.0f);
    
	ProgressTracker->AddProgressByPercentage(PctAddedPerPress);
}

// Called when the game starts or when spawned
void APressStation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APressStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

