// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"

#include "ViewportInteractionTypes.h"
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

int AInteractable::GetCapacity()
{
	return Capacity;
}

AInteractable* AInteractable::GetOfferedInteractable()
{
	if (OfferedInteractable.IsValid() && OfferedInteractable->Moveable)
	{
		AInteractable* Temp = OfferedInteractable.Get();
		OfferedInteractable = this;
		Capacity++;
		return Temp;
	}
	return nullptr;
}


bool AInteractable::AddInteractable(AInteractable* InteractableToAdd)
{
	if (Capacity <= 0 || !InteractableToAdd) { return false; }

	OfferedInteractable = InteractableToAdd;
	Capacity--;

	// Physics/Collision Cleanup
	InteractableToAdd->DisableInteractable();
	//InteractableToAdd->GetRootComponent()->SetSimulatePhysics(false);

	// Attachment
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);
	InteractableToAdd->AttachToComponent(GetRootComponent(), Rules); 
	// Tip: Use a specific socket name if your mesh has one, e.g., GetMesh(), "SlotSocket"

	return true;
}
// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

