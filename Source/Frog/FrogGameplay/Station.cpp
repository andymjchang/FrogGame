// Fill out your copyright notice in the Description page of Project Settings.

#include "Station.h"
#include "Components/PrimitiveComponent.h"
#include "FrogGameplay/InteractableComponent.h"

AStation::AStation()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AStation::BeginPlay()
{
	Super::BeginPlay();
}

void AStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}