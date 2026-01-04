// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"

#include "InteractableData.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameUI/Interactables/InventoryWidget.h"

AInteractable::AInteractable()
{
	// Networking
	bReplicates = true;

	// Root Component
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	SetRootComponent(RootSceneComponent);
	
	// Interact Hitbox
	PrimaryActorTick.bCanEverTick = true;
	InteractHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractHitBox"));
	InteractHitBox->SetupAttachment(RootComponent);
	InteractHitBox->SetCollisionProfileName(TEXT("ItemHitBox"));
	InteractHitBox->InitBoxExtent(FVector(128.f, 128.f, 128.f));
	InteractHitBox->SetRelativeLocation(FVector(0.0f, 0.0f, 128.0f));
	
	// Inventory Widget
	InventoryWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InventoryWidgetComponent"));
	InventoryWidgetComponent->SetupAttachment(RootComponent);
	InventoryWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 600.0f));
	InventoryWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	InventoryWidgetComponent->SetDrawSize(FIntPoint(100, 100));
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
	if (!IsValid(Data) || !IsValid(InteractableToAdd)) return false;
	
	UInteractableData* InteractableData = InteractableToAdd->GetData();
	if (!IsValid(InteractableData)) return false;
	
	if (!InteractableData->GetCompatibleTags().HasAny(Data->GetOwnedTags())) return false;
	if (GetInventorySize() >= Data->GetMaxCapacity()) return false;
	
	Inventory.Add(InteractableToAdd);
	// Inventory Widget
	if (UInventoryWidget* InventoryWidget = Cast<UInventoryWidget>(InventoryWidgetComponent->GetWidget()))
	{
		InventoryWidget->UpdateInventoryWidget(Inventory);
	}
	// Attachment
	InteractableToAdd->DisableInteractable();
	const FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, false);
	InteractableToAdd->AttachToComponent(GetRootComponent(), Rules); 

	return true;
}

void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(Data)) Inventory.Reserve(Data->GetMaxCapacity());
	OfferedInteractable = this;
}

