// Fill out your copyright notice in the Description page of Project Settings.


#include "TrashCan.h"

#include "ContainerComponent.h"


ATrashCan::ATrashCan()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATrashCan::BeginPlay()
{
	Super::BeginPlay();
	
	USceneComponent* AttachPoint = ContainerComponent->GetAttachPoint();
	if (IsValid(AttachPoint))
	{
		AttachPoint->SetVisibility(false, true);
	}
}

void ATrashCan::HandleAddedToInventory(AItem* Interactable)
{
	Super::HandleAddedToInventory(Interactable);
	if (!IsValid(Interactable)) return;
	
	ContainerComponent->TryRemoveFromInventory(Interactable);
	Interactable->Destroy();
}