// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingItem.h"

#include "AI/FrogAIController.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"


AMovingItem::AMovingItem()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	// Root Component
	CollisionComponent = GetCapsuleComponent();
	CollisionComponent->SetCollisionProfileName(TEXT("Pawn"));
	CollisionComponent->InitCapsuleSize(75.f, 150.f);
	
	// Interact Hitbox
	InteractHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractHitBox"));
	InteractHitBox->SetupAttachment(RootComponent);
	InteractHitBox->SetCollisionProfileName(TEXT("InteractListen"));
	InteractHitBox->InitBoxExtent(FVector(150.f, 150.f, 150.f));
	
	// Skeletal Mesh Component
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
}

void AMovingItem::HandleAddedToAnotherInventory_Implementation()
{
	if (HasAuthority())
	{
		if (AFrogAIController* AIController = Cast<AFrogAIController>(GetController()))
		{
			if (UStateTreeAIComponent* StateTree = AIController->GetStateTree())
			{
				StateTree->StopLogic(FString(""));
			}
			AIController->StopMovement();
		}
	}
	
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetMesh()->Stop();
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

void AMovingItem::EventAddedToAnotherInventory()
{
	IItemInterface::EventAddedToAnotherInventory();
	HandleAddedToAnotherInventory();
}

void AMovingItem::OnRep_bIsHitboxEnabled()
{
	if (IsValid(InteractHitBox) && IsValid(CollisionComponent))
	{
		// FLOG(TEXT("ID: %d bIsHitboxEnabled: %d"), GetWorld()->GetNetMode(), bIsHitboxEnabled);
		if (bIsHitboxEnabled)
		{
			InteractHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			// CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			InteractHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			// CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

