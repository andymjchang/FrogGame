// Fill out your copyright notice in the Description page of Project Settings.

#include "OrderCounter.h"

#include "ContainerComponent.h"
#include "ItemData.h"
#include "GameState/FrogGameState.h"

AOrderCounter::AOrderCounter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AOrderCounter::BeginPlay()
{
	Super::BeginPlay();
	
	if (const UWorld* World = GetWorld())
	{
		GameState = World->GetGameState<AFrogGameState>();
	}
}

void AOrderCounter::HandleAddedToInventory(AItem* Interactable)
{
	Super::HandleAddedToInventory(Interactable);
	if (!IsValid(Interactable)) return;
	
	if (GameState.IsValid())
	{
		GameState->ServerAddMoney(Interactable->GetData()->GetSellPrice());;
	}
    
	ContainerComponent->ClearInventory();
}
