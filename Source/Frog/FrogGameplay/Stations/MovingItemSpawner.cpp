// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingItemSpawner.h"

#include "FrogGameplay/Core/ItemInterface.h"
#include "FrogGameplay/Core/MovingItem.h"


AMovingItemSpawner::AMovingItemSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMovingItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnMovingItem();
}

void AMovingItemSpawner::SpawnMovingItem()
{
	if (!HasAuthority()) return;
	
	if (const TScriptInterface<IItemInterface> NewItem = SpawnItem(InteractableClassToSpawn, GetRootComponent()->GetComponentTransform()))
	{
		if (AMovingItem* NewMovingItem = Cast<AMovingItem>(NewItem.GetObject()))
		{
			NewMovingItem->OnDestroyed.AddDynamic(this, &AMovingItemSpawner::StartSpawnTimer);
		}
	}
}

void AMovingItemSpawner::StartSpawnTimer(AActor* DestroyedActor)
{
	if (!HasAuthority()) return;
	
	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			ReplenishTimerHandle, 
			this, 
			&AMovingItemSpawner::SpawnMovingItem, 
			ReplenishDelay, 
			false
		);
	}
}