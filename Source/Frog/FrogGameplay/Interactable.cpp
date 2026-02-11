// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"

#include "Components/BoxComponent.h"

AInteractable::AInteractable()
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

void AInteractable::Interact()
{
}

void AInteractable::StartHighlight()
{
	if (IsValid(InteractableMesh) && IsValid(OverlayMaterial))
	{
		InteractableMesh->SetOverlayMaterial(OverlayMaterial);
	}
}

void AInteractable::StopHighlight()
{
	if (IsValid(InteractableMesh))
	{
		InteractableMesh->SetOverlayMaterial(nullptr);
	}
}

void AInteractable::EnableInteractable()
{
	if (IsValid(InteractHitBox)) InteractHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AInteractable::DisableInteractable()
{
	if (IsValid(InteractHitBox)) InteractHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}