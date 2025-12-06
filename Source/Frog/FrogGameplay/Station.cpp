// Fill out your copyright notice in the Description page of Project Settings.

#include "Station.h"
#include "Components/PrimitiveComponent.h"
#include "FrogGameplay/InteractableComponent.h"

AStation::AStation()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create the InteractableComponent
	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
    
	// Attach the hitbox to root
	InteractableComponent->SetupAttachment(GetRootComponent());
}

void AStation::BeginPlay()
{
	Super::BeginPlay();
    
	if (InteractableComponent)
	{
		InteractableComponent->StartInteract.AddDynamic(this, &AStation::OnStartInteract);
		InteractableComponent->StopInteract.AddDynamic(this, &AStation::OnStopInteract);
	}
}

void AStation::OnStartInteract(AActor* OtherActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Started interacting with: %s"), *OtherActor->GetName()));
}

void AStation::OnStopInteract(AActor* OtherActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Stopped interacting with: %s"), *OtherActor->GetName()));
}

void AStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UInteractableComponent* AStation::GetInteractableComponent()
{
	return InteractableComponent;
}