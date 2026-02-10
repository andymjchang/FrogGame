// Fill out your copyright notice in the Description page of Project Settings.

#include "Spawner.h"
#include "Engine/World.h"

ASpawner::ASpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
	OnRemovedFromInventory.BindDynamic(this, &ASpawner::HandleInteractableRemoved);
	SpawnAndReplenish();
}

void ASpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ASpawner::HandleInteractableRemoved(AItem* AddedInteractable)
{
	// Could add support for timed respawns
	SpawnAndReplenish();
}

void ASpawner::SpawnAndReplenish()
{
	if (!IsValid(InteractableClassToSpawn)) return;

	UWorld* World = GetWorld();
	if (!World) return;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AItem* NewItem = World->SpawnActor<AItem>(
		InteractableClassToSpawn, 
		AttachPoint->GetComponentTransform(), 
		SpawnParams
	);

	if (NewItem)
	{
		NewItem->AttachToComponent(AttachPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
		TryAddToInventory(NewItem);
		OfferedInteractable = NewItem;
	}
}