// Fill out your copyright notice in the Description page of Project Settings.

#include "Spawner.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "FrogGameplay/Core/ContainerComponent.h"

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

void ASpawner::HandleRemovedFromInventory(AItem* Interactable)
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
	if (!HasAuthority()) return;
    
	UWorld* World = GetWorld();
	if (!World) return;
    
	if (!IsValid(InteractableClassToSpawn)) return;
	if (!IsValid(ContainerComponent)) return;
	if (!ContainerComponent->GetAllowAdd()) return;
    
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