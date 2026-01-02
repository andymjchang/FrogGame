// Fill out your copyright notice in the Description page of Project Settings.

#include "Station.h"
#include "Components/PrimitiveComponent.h"
#include "FrogCharacter/FrogCharacter.h"

AStation::AStation()
{
	
}

void AStation::BeginPlay()
{
	Super::BeginPlay();
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
