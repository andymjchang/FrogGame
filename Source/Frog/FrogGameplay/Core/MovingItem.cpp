// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingItem.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"


AMovingItem::AMovingItem()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	// Root Component
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;
	
	// Interact Hitbox
	InteractHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractHitBox"));
	InteractHitBox->SetupAttachment(RootComponent);
	InteractHitBox->SetCollisionProfileName(TEXT("InteractListen"));
	InteractHitBox->InitBoxExtent(FVector(150.f, 150.f, 150.f));
	InteractHitBox->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	
	// Skeletal Mesh Component
	InteractableMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("InteractableSkeletalMesh"));
	InteractableMesh->SetupAttachment(RootComponent);
	InteractableMesh->SetCollisionProfileName(TEXT("NoCollision"));
	
	// Capsule Collision
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetCollisionProfileName(TEXT("Pawn"));
	CollisionComponent->InitCapsuleSize(75.f, 150.f);
	CollisionComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
}

void AMovingItem::BeginPlay()
{
	Super::BeginPlay();
	OfferedInteractable = this;
}

void AMovingItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AMovingItem, bIsHitboxEnabled);
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
	if (!HasAuthority()) return;

	bIsHitboxEnabled = true;
	OnRep_bIsHitboxEnabled();
}

void AMovingItem::DisableHitbox()
{
	if (!HasAuthority()) return;
	
	bIsHitboxEnabled = false;
	OnRep_bIsHitboxEnabled();
}

void AMovingItem::OnRep_bIsHitboxEnabled()
{
	if (IsValid(InteractHitBox) && IsValid(CollisionComponent))
	{
		// FLOG(TEXT("ID: %d bIsHitboxEnabled: %d"), GetWorld()->GetNetMode(), bIsHitboxEnabled);
		if (bIsHitboxEnabled)
		{
			InteractHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			InteractHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

