// Fill out your copyright notice in the Description page of Project Settings.

#include "OrderCounter.h"

#include "GameState/FrogGameState.h"

AOrderCounter::AOrderCounter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AOrderCounter::BeginPlay()
{
	Super::BeginPlay();
    
	OnAddedToInventory.AddUObject(this, &AOrderCounter::HandleInteractableAdded);
	
	if (IsValid(AttachPoint))
	{
		AttachPoint->SetVisibility(false, true);
	}
	
	if (const UWorld* World = GetWorld())
	{
		GameState = World->GetGameState<AFrogGameState>();
	}
}

void AOrderCounter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	OnAddedToInventory.RemoveAll(this);

	Super::EndPlay(EndPlayReason);
}

void AOrderCounter::HandleInteractableAdded(AInteractable* Interactable)
{
	if (!IsValid(Interactable)) return;
	
	if (GameState.IsValid())
	{
		GameState->ServerAddScore(10);
	}
    
	TryRemoveFromInventory(Interactable);
	Interactable->Destroy();
}