// Fill out your copyright notice in the Description page of Project Settings.

#include "OrderCounter.h"

#include "ContainerComponent.h"
#include "InteractableData.h"
#include "GameState/FrogGameState.h"

AOrderCounter::AOrderCounter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AOrderCounter::BeginPlay()
{
	Super::BeginPlay();

	USceneComponent* AttachPoint = ContainerComponent->GetAttachPoint();
	if (IsValid(AttachPoint))
	{
		AttachPoint->SetVisibility(false, true);
	}
	
	if (const UWorld* World = GetWorld())
	{
		GameState = World->GetGameState<AFrogGameState>();
	}
}

void AOrderCounter::HandleAddedToInventory(AInteractable* Interactable)
{
	Super::HandleRemovedFromInventory(Interactable);
	if (!IsValid(Interactable)) return;
	
	if (GameState.IsValid())
	{
		GameState->ServerAddMoney(Interactable->GetData()->GetSellPrice());;
	}
    
	ContainerComponent->TryRemoveFromInventory(Interactable);
	Interactable->Destroy();
}
