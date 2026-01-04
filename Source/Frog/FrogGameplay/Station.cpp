// Fill out your copyright notice in the Description page of Project Settings.

#include "Station.h"
#include "Components/PrimitiveComponent.h"
#include "Components/WidgetComponent.h"
#include "FrogCharacter/FrogCharacter.h"
#include "GameUI/Interactables/InteractableWidgetComponent.h"

AStation::AStation()
{
	// Inventory Widget
	ProgressBarWidgetComponent = CreateDefaultSubobject<UInteractableWidgetComponent>(TEXT("UBillboardWidgetComponent"));
	ProgressBarWidgetComponent->SetupAttachment(RootComponent);
	ProgressBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -600.0f));
	ProgressBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	ProgressBarWidgetComponent->SetDrawSize(FIntPoint(100, 20));
}

void AStation::BeginPlay()
{
	Super::BeginPlay();
	ProgressBarWidgetComponent->SetVisibility(false);
}

void AStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStation::OnInteract(AFrogCharacter* Interactor)
{
	if (GEngine)
	{
		const FString DebugMessage = FString::Printf(TEXT("interacted with station "));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, DebugMessage);
	}
}
