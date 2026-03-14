// Fill out your copyright notice in the Description page of Project Settings.


#include "Counter.h"

ACounter::ACounter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	InteractableMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

void ACounter::HandleAddedToInventory(AItem* Interactable)
{
	Super::HandleAddedToInventory(Interactable);
	if (!HasAuthority()) return;
	if (!IsValid(Interactable)) return;
	
	OfferedInteractable = Interactable;
}

