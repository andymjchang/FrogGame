// Fill out your copyright notice in the Description page of Project Settings.


#include "Container.h"

#include "Frog.h"
#include "ItemData.h"
#include "GameUI/Interactables/InteractableWidgetComponent.h"
#include "GameUI/Interactables/InventoryWidget.h"

AContainer::AContainer()
{
	PrimaryActorTick.bCanEverTick = false;
	
	// Inventory Widget
	InventoryWidgetComponent = CreateDefaultSubobject<UInteractableWidgetComponent>(TEXT("InventoryWidgetComponent"));
	InventoryWidgetComponent->SetupAttachment(RootComponent);
	InventoryWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 600.0f));
	InventoryWidgetComponent->SetDrawSize(FIntPoint(100, 100));
	
	// Attach point
	AttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("AttachPoint"));
	AttachPoint->SetupAttachment(RootComponent);
}

void AContainer::ClearInventory()
{
	Inventory.Empty();
	UpdateInventoryWidget();
}

bool AContainer::TryAddToInventory(AInteractable* InteractableToAdd)
{
	if (!IsValid(Data) || !IsValid(InteractableToAdd)) return false;
	if (GetInventorySize() >= Data->GetMaxCapacity()) return false;
	if (!InteractableToAdd->HasMatchingInteractableTag(Data->GetAcceptedTags())) return false;
	
	Inventory.Add(InteractableToAdd);
	// Attachment
	InteractableToAdd->DisableInteractable();
	const FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, false);
	InteractableToAdd->AttachToComponent(AttachPoint, Rules); 
	
	UpdateInventoryWidget();
	OnAddedToInventory.ExecuteIfBound(InteractableToAdd);
	
	return true;
}

bool AContainer::TryAddContainerContentsToInventory(AContainer* ContainerToAdd)
{
	if (!IsValid(ContainerToAdd)) return false;
	if (ContainerToAdd->GetInventorySize() <= 0) return false;
	if (GetInventorySize() + ContainerToAdd->GetInventorySize() > Data->GetMaxCapacity()) return false;
	
	const TArray<AInteractable*>& InInventory = ContainerToAdd->GetInventory();
	// Check if all ingredients are valid
	for (const AInteractable* InventoryIndex : InInventory)
	{
		if (!InventoryIndex->HasMatchingInteractableTag(Data->GetAcceptedTags()))
		{
			return false;
		}
	}
	
	// Add to inventory (Iterating Backwards)
	for (int32 i = InInventory.Num() - 1; i >= 0; --i)
	{
		TryAddToInventory(InInventory[i]);
	}
	
	ContainerToAdd->ClearInventory();
	UpdateInventoryWidget();
	
	return true;
}

bool AContainer::TryRemoveFromInventory(AInteractable* InteractableToRemove)
{
	if (!IsValid(InteractableToRemove)) return false;
	
	if (Inventory.Remove(InteractableToRemove) > 0)
	{
		// // Update offered interactable
		// if (Inventory.Num() > 0)
		// {
		// 	// Offer the last item in inventory
		// 	OfferedInteractable = Inventory.Last();
		// }
		// else
		// {
		// 	// Inventory is empty, offer self
		// 	OfferedInteractable = this;
		// }
		OfferedInteractable = this;
		
		UpdateInventoryWidget();
		OnRemovedFromInventory.ExecuteIfBound(InteractableToRemove);
		return true;
	}
	return false;
}

void AContainer::UpdateInventoryWidget()
{
	if (UInventoryWidget* InventoryWidget = Cast<UInventoryWidget>(InventoryWidgetComponent->GetWidget()))
	{
		InventoryWidget->UpdateInventoryWidget(Inventory);
	}
}

void AContainer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (IsValid(Data)) 
	{
		Inventory.Reserve(Data->GetMaxCapacity());
	}
}

