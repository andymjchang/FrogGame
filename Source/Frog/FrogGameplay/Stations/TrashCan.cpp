// Fill out your copyright notice in the Description page of Project Settings.


#include "TrashCan.h"

#include "FrogGameplay/Core/ContainerComponent.h"


ATrashCan::ATrashCan()
{
	PrimaryActorTick.bCanEverTick = false;
	
	// Interactable Mesh 
	InteractableMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

void ATrashCan::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(ContainerComponent))
	{
		ContainerComponent->SetVisibility(false, true);
	}
}

void ATrashCan::HandleAddedToInventory(AItem* Interactable)
{
	Super::HandleAddedToInventory(Interactable);
	if (!HasAuthority()) return;
	if (!IsValid(Interactable)) return;
	
	ContainerComponent->TryRemoveFromInventory(Interactable);
	Interactable->Destroy();
}