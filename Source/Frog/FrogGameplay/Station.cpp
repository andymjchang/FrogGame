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
	
}

bool AStation::TryAddItem(AItem* Item)
{
	if (CurrentCapacity < MaxCapacity)
	{
		Items.Add(Item);
		CurrentCapacity++;
		return true;
	}
	return false;
}
