// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingItem.h"

#include "Components/BoxComponent.h"


AMovingItem::AMovingItem()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	// Root Component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	
	// Interact Hitbox
	InteractHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractHitBox"));
	InteractHitBox->SetupAttachment(RootComponent);
	InteractHitBox->SetCollisionProfileName(TEXT("InteractListen"));
	InteractHitBox->InitBoxExtent(FVector(150.f, 150.f, 150.f));
	InteractHitBox->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	InteractHitBox->SetIsReplicated(true);
}

void AMovingItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

FVector AMovingItem::GetInteractableLocation() const
{
	return GetRootComponent()->GetComponentLocation();
}

void AMovingItem::StartInteract()
{
	IInteractableInterface::StartInteract();
}

void AMovingItem::EnableHitbox()
{
}

void AMovingItem::DisableHitbox()
{
}

void AMovingItem::OnRep_bIsHitboxEnabled()
{
}

void AMovingItem::BeginPlay()
{
	Super::BeginPlay();
	OfferedInteractable = this;
}
