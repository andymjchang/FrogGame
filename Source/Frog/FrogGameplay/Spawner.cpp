// Fill out your copyright notice in the Description page of Project Settings.

#include "Spawner.h"

#include "ContainerComponent.h"
#include "Frog.h"
#include "Engine/World.h"

ASpawner::ASpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnAndReplenish();
}

void ASpawner::HandleRemovedFromInventory(AInteractable* Interactable)
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
	USceneComponent* AttachPoint = ContainerComponent->GetAttachPoint();
	if (!IsValid(AttachPoint)) return;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AInteractable* NewItem = World->SpawnActor<AInteractable>(
		InteractableClassToSpawn, 
		AttachPoint->GetComponentTransform(),
		// FTransform::Identity,
		SpawnParams
	);

	if (NewItem)
	{
		NewItem->AttachToComponent(AttachPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
		ContainerComponent->TryAddToInventory(NewItem);
		OfferedInteractable = NewItem;
	}
}