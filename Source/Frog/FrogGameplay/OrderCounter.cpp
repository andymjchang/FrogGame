// Fill out your copyright notice in the Description page of Project Settings.

#include "OrderCounter.h"

#include "ItemData.h"
#include "GameState/FrogGameState.h"

AOrderCounter::AOrderCounter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AOrderCounter::BeginPlay()
{
	Super::BeginPlay();
    
	OnAddedToInventory.BindDynamic(this, &AOrderCounter::HandleInteractableAdded);
	
	if (IsValid(AttachPoint))
	{
		AttachPoint->SetVisibility(false, true);
	}
	
	if (const UWorld* World = GetWorld())
	{
		GameState = World->GetGameState<AFrogGameState>();
	}
}

void AOrderCounter::HandleInteractableAdded(AContainer* Interactable)
{
	if (!IsValid(Interactable)) return;
	
	if (GameState.IsValid())
	{
		GameState->ServerAddMoney(Interactable->GetData()->GetSellPrice());;
	}
    
	TryRemoveFromInventory(Interactable);
	Interactable->Destroy();
}