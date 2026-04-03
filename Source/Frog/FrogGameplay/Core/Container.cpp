// Fill out your copyright notice in the Description page of Project Settings.


#include "Container.h"

#include "ContainerComponent.h"
#include "Frog.h"
#include "GameUI/Interactables/InteractableWidgetComponent.h"

AContainer::AContainer()
{
	PrimaryActorTick.bCanEverTick = false;
	
	ContainerComponent = CreateDefaultSubobject<UContainerComponent>(TEXT("ContainerComponent"));
	ContainerComponent->SetupAttachment(GetRootComponent());
	
	InventoryWidgetComponent = CreateDefaultSubobject<UInteractableWidgetComponent>(TEXT("InventoryWidgetComponent"));
	InventoryWidgetComponent->SetupAttachment(RootComponent);
	InventoryWidgetComponent->SetDrawSize(FIntPoint(100, 100));
}

void AContainer::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(ContainerComponent))
	{
		ContainerComponent->OnAddedToInventory.AddDynamic(this, &AContainer::HandleAddedToInventory);
		ContainerComponent->OnRemovedFromInventory.AddDynamic(this, &AContainer::HandleRemovedFromInventory);
		ContainerComponent->Initialize(Data, InventoryWidgetComponent, InteractableMesh.Get());
	}
}

void AContainer::Destroyed()
{
	if (HasAuthority() && IsValid(ContainerComponent))
	{
		ContainerComponent->ClearInventory();
	}

	Super::Destroyed();
}

void AContainer::HandleAddedToInventory(const TScriptInterface<IItemInterface>& Interactable)
{

}

void AContainer::HandleRemovedFromInventory(const TScriptInterface<IItemInterface>& Interactable)
{
	OfferedInteractable = this;
}



