// Fill out your copyright notice in the Description page of Project Settings.


#include "Container.h"

#include "ContainerComponent.h"
#include "GameUI/Interactables/InteractableWidgetComponent.h"

AContainer::AContainer()
{
	PrimaryActorTick.bCanEverTick = false;
	
	ContainerComponent = CreateDefaultSubobject<UContainerComponent>(TEXT("ContainerComponent"));
	ContainerComponent->SetupAttachment(GetRootComponent());
	
	InventoryWidgetComponent = CreateDefaultSubobject<UInteractableWidgetComponent>(TEXT("InventoryWidgetComponent"));
	InventoryWidgetComponent->SetupAttachment(RootComponent);
	InventoryWidgetComponent->SetRelativeLocation(FVector(0.f, 0.0f, 600.0f));
	InventoryWidgetComponent->SetDrawSize(FIntPoint(100, 100));
}

void AContainer::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(ContainerComponent))
	{
		ContainerComponent->OnAddedToInventory.BindDynamic(this, &AContainer::HandleAddedToInventory);
		ContainerComponent->OnRemovedFromInventory.BindDynamic(this, &AContainer::HandleRemovedFromInventory);
		ContainerComponent->Initialize(Data, InventoryWidgetComponent);
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

void AContainer::HandleAddedToInventory(AItem* Interactable)
{

}

void AContainer::HandleRemovedFromInventory(AItem* Interactable)
{
	OfferedInteractable = this;
}



