// Fill out your copyright notice in the Description page of Project Settings.


#include "Container.h"

#include "ContainerComponent.h"

AContainer::AContainer()
{
	PrimaryActorTick.bCanEverTick = false;
	
	ContainerComponent = CreateDefaultSubobject<UContainerComponent>(TEXT("ContainerComponent"));
	ContainerComponent->SetupAttachment(GetRootComponent());
}

void AContainer::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(ContainerComponent))
	{
		ContainerComponent->OnAddedToInventory.BindDynamic(this, &AContainer::HandleAddedToInventory);
		ContainerComponent->OnRemovedFromInventory.BindDynamic(this, &AContainer::HandleRemovedFromInventory);
		ContainerComponent->Initialize(Data);
	}
}

void AContainer::HandleRemovedFromInventory(AInteractable* Interactable)
{
	OfferedInteractable = this;
}

void AContainer::HandleAddedToInventory(AInteractable* Interactable)
{
}





