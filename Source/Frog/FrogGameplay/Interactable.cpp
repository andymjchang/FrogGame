// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"

#include "InteractableData.h"
#include "Components/BoxComponent.h"
#include "GameUI/Interactables/InteractableWidgetComponent.h"
#include "GameUI/Interactables/InventoryWidget.h"

AInteractable::AInteractable()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// Root Component
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	SetRootComponent(RootSceneComponent);
	
	// Interact Hitbox
	InteractHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractHitBox"));
	InteractHitBox->SetupAttachment(RootComponent);
	InteractHitBox->SetCollisionProfileName(TEXT("InteractListen"));
	InteractHitBox->InitBoxExtent(FVector(128.f, 128.f, 128.f));
	InteractHitBox->SetRelativeLocation(FVector(0.0f, 0.0f, 128.0f));
	
	// Inventory Widget
	InventoryWidgetComponent = CreateDefaultSubobject<UInteractableWidgetComponent>(TEXT("InventoryWidgetComponent"));
	InventoryWidgetComponent->SetupAttachment(RootComponent);
	InventoryWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 600.0f));
	InventoryWidgetComponent->SetDrawSize(FIntPoint(100, 100));
	
	// Attach point
	AttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("AttachPoint"));
	AttachPoint->SetupAttachment(RootComponent);
}

void AInteractable::EnableInteractable()
{
	if (IsValid(InteractHitBox)) InteractHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AInteractable::DisableInteractable()
{
	if (IsValid(InteractHitBox)) InteractHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AInteractable::ClearInventory()
{
	Inventory.Empty();
}

bool AInteractable::HasMatchingInteractableTag(const FGameplayTagContainer& AcceptedTags) const
{
	if (AcceptedTags.IsEmpty()) return false;
	if (!IsValid(Data)) return false;

	const FGameplayTagContainer& OwnedTags = Data->GetOwnedTags();
	for (const FGameplayTag& OwnedTag : OwnedTags)
	{
		if (OwnedTag.MatchesAny(AcceptedTags))
		{
			return true;
		}
	}

	return false;
}

bool AInteractable::TryAddToInventory(AInteractable* InteractableToAdd)
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
	OnAddedToInventory.Broadcast(InteractableToAdd);
	
	return true;
}

bool AInteractable::TryAddContainerToInventory(AInteractable* ContainerToAdd)
{
	if (!IsValid(ContainerToAdd)) return false;
	if (ContainerToAdd->GetInventorySize() <= 0) return false;
	if (GetInventorySize() + ContainerToAdd->GetInventorySize() > Data->GetMaxCapacity()) return false;
	
	const TArray<AInteractable*>& InInventory = ContainerToAdd->GetInventory();
	// Check if all ingredients are valid
	for (AInteractable* InventoryIndex : InInventory)
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
	ContainerToAdd->UpdateInventoryWidget();
	UpdateInventoryWidget();
	
	return true;
}

bool AInteractable::TryRemoveFromInventory(AInteractable* InteractableToRemove)
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
		OnRemovedFromInventory.Broadcast(InteractableToRemove);
		
		return true;
	}
	return false;
}

void AInteractable::UpdateInventoryWidget()
{
	if (UInventoryWidget* InventoryWidget = Cast<UInventoryWidget>(InventoryWidgetComponent->GetWidget()))
	{
		InventoryWidget->UpdateInventoryWidget(Inventory);
	}
}

void AInteractable::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (IsValid(Data)) 
	{
		Inventory.Reserve(Data->GetMaxCapacity());
	}
}

void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	
	OfferedInteractable = this;
}

