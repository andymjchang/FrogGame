// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "GameplayTagContainer.h"
#include "ItemData.h"
#include "Components/BoxComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;
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
	
	// Static Mesh Component
	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InteractableMesh"));
	InteractableMesh->SetupAttachment(RootComponent);
}

bool AItem::HasMatchingInteractableTag(const FGameplayTagContainer& AcceptedTags) const
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

void AItem::StartInteract()
{
}

void AItem::StopInteract()
{
}

void AItem::StartHighlight()
{
	if (IsValid(InteractableMesh) && IsValid(OverlayMaterial))
	{
		InteractableMesh->SetOverlayMaterial(OverlayMaterial);
	}
}

void AItem::StopHighlight()
{
	if (IsValid(InteractableMesh))
	{
		InteractableMesh->SetOverlayMaterial(nullptr);
	}
}

FVector AItem::GetInteractableLocation()
{
	return RootSceneComponent->GetComponentLocation();
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	OfferedInteractable = this;
}

void AItem::EnableInteractable()
{
	if (IsValid(InteractHitBox)) InteractHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AItem::DisableInteractable()
{
	if (IsValid(InteractHitBox)) InteractHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}