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

void ATrashCan::HandleAddedToInventory(const TScriptInterface<IItemInterface>& Interactable)
{
	Super::HandleAddedToInventory(Interactable);
	if (!HasAuthority()) return;
	
	AActor* InteractableActor = Cast<AActor>(Interactable.GetObject());
	if (!IsValid(InteractableActor)) return;
	
	ContainerComponent->TryRemoveFromInventory(Interactable);
	InteractableActor->Destroy();
}