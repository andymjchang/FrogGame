// Fill out your copyright notice in the Description page of Project Settings.

#include "Spawner.h"

#include "ContainerComponent.h"
#include "Engine/World.h"

ASpawner::ASpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	ContainerComponent->SetShowInventoryWidget(false);
}

void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnAndReplenish();
}

void ASpawner::HandleRemovedFromInventory(AItem* Interactable)
{
	Super::HandleRemovedFromInventory(Interactable);
	SpawnAndReplenish();
}

void ASpawner::SpawnAndReplenish()
{
	UWorld* World = GetWorld();
	if (!World) return;
	
	if (!IsValid(InteractableClassToSpawn)) return;
	if (!IsValid(ContainerComponent)) return;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AItem* NewItem = World->SpawnActor<AItem>(
		InteractableClassToSpawn, 
		ContainerComponent->GetComponentTransform(),
		SpawnParams
	);

	if (IsValid(NewItem))
	{
		NewItem->AttachToComponent(ContainerComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
		ContainerComponent->TryAddToInventory(NewItem);
		OfferedInteractable = NewItem;
	}
}