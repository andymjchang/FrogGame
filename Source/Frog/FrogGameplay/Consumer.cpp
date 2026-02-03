// Fill out your copyright notice in the Description page of Project Settings.


#include "Consumer.h"


AConsumer::AConsumer()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AConsumer::BeginPlay()
{
	Super::BeginPlay();
	
	OnAddedToInventory.AddUObject(this, &AConsumer::HandleInteractableAdded);
	AttachPoint->SetVisibility(false, true);
}

void AConsumer::HandleInteractableAdded(AInteractable* Interactable)
{
	if (!IsValid(Interactable)) return ;
	
	TryRemoveFromInventory(Interactable);
	Interactable->Destroy();
}