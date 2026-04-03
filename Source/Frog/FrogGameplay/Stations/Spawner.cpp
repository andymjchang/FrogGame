// Fill out your copyright notice in the Description page of Project Settings.

#include "Spawner.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "FrogGameplay/Core/ContainerComponent.h"
#include "FrogGameplay/Core/MovingItem.h"

ASpawner::ASpawner()
{
	PrimaryActorTick.bCanEverTick = false;
    
	InteractableMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

void ASpawner::BeginPlay()
{
	Super::BeginPlay();
    
	if (HasAuthority())
	{
		SpawnAndReplenish();
	}
}

void ASpawner::HandleRemovedFromInventory(const TScriptInterface<IItemInterface>& Interactable)
{
	Super::HandleRemovedFromInventory(Interactable);
    
	if (HasAuthority())
	{
		if (const UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(
				ReplenishTimerHandle, 
				this, 
				&ASpawner::SpawnAndReplenish, 
				ReplenishDelay, 
				false
			);
		}
	}
}

void ASpawner::SpawnAndReplenish()
{
	OfferedInteractable = this;
	
	if (IsValid(ContainerComponent))
	{
		if (const TScriptInterface<IItemInterface> NewItem = SpawnItem(InteractableClassToSpawn, ContainerComponent->GetComponentTransform()))
		{
			if (ContainerComponent->TryAddToInventory(NewItem))
            {
            	OfferedInteractable = NewItem;
            }
		}
	}
}