// Fill out your copyright notice in the Description page of Project Settings.


#include "Consumer.h"

#include "ContainerComponent.h"


AConsumer::AConsumer()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AConsumer::BeginPlay()
{
	Super::BeginPlay();
	
	USceneComponent* AttachPoint = ContainerComponent->GetAttachPoint();
	if (IsValid(AttachPoint))
	{
		AttachPoint->SetVisibility(false, true);
	}
}

void AConsumer::HandleAddedToInventory(AInteractable* Interactable)
{
	Super::HandleAddedToInventory(Interactable);
	if (!IsValid(Interactable)) return;
	
	USceneComponent* AttachPoint = ContainerComponent->GetAttachPoint();
	if (IsValid(AttachPoint))
	{
		AttachPoint->SetVisibility(false, true);
	}
	
	ContainerComponent->TryRemoveFromInventory(Interactable);
	
	Interactable->Destroy();
}