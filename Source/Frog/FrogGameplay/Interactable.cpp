// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"
#include "Components/BoxComponent.h"


// Sets default values
AInteractable::AInteractable()
{
	// Networking
	bReplicates = true;

	PrimaryActorTick.bCanEverTick = true;
	OfferedInteractable = this;
	InteractHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractHitBox"));
	InteractHitBox->SetupAttachment(GetRootComponent());
	InteractHitBox->SetCollisionProfileName(TEXT("ItemHitBox"));
}

void AInteractable::EnableInteractable()
{
	if (IsValid(InteractHitBox)) InteractHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AInteractable::DisableInteractable()
{
	if (IsValid(InteractHitBox)) InteractHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool AInteractable::TryAddToInventory(AInteractable* InteractableToAdd)
{
	if (GetInventorySize() >= MaxCapacity || !IsValid(InteractableToAdd)) return false;
	if (!InteractableToAdd->GetCompatibleItemTags().HasAny(OwnedInteractableTags)) return false;
	
	Inventory.Add(InteractableToAdd);

	// Attachment
	InteractableToAdd->DisableInteractable();
	const FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, false);
	InteractableToAdd->AttachToComponent(GetRootComponent(), Rules); 

	return true;
}

void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	Inventory.Reserve(MaxCapacity);
}

