// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "Components/BoxComponent.h"
#include "Components/ProgressBar.h"
#include "FrogGameplay/ProgressTrackingComponent.h"


ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Root Component
	USceneComponent* DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	RootComponent = DefaultRoot;

	// Hitbox
	Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	Hitbox->SetupAttachment(RootComponent);
	Hitbox->SetBoxExtent(FVector(64.f, 256.f, 128.f));
	Hitbox->SetRelativeLocation(FVector(0.0f, 0.0f, 128.0f));
	Hitbox->SetCollisionProfileName(TEXT("InteractListen"));
	
	// Progress Tracker
	ProgressTracker = CreateDefaultSubobject<UProgressTrackingComponent>(TEXT("ProgressTrackingComponent"));
	
}

void ADoor::SetActive(const bool bIsTrue)
{
	if (!IsValid(Hitbox)) return;
	
	if (bIsTrue)
	{
		Hitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		Hitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
