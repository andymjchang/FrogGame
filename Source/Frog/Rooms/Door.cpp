// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "Components/BoxComponent.h"
#include "FrogGameplay/Core/ProgressTrackingComponent.h"
#include "GameState/FrogGameState.h"
#include "GameUI/Interactables/InteractableWidgetComponent.h" 

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	// Root Component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	SetRootComponent(RootComponent);
	
	// Hitbox
	InteractHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	InteractHitbox->SetupAttachment(RootComponent);
	InteractHitbox->SetBoxExtent(FVector(64.f, 256.f, 128.f));
	InteractHitbox->SetRelativeLocation(FVector(0.0f, 0.0f, 128.0f));
	InteractHitbox->SetCollisionProfileName(TEXT("InteractListen"));
	
	// Static Mesh Component
	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InteractableMesh"));
	InteractableMesh->SetupAttachment(RootComponent);
	InteractableMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));	
	
	// Progress Tracker
	ProgressTracker = CreateDefaultSubobject<UProgressTrackingComponent>(TEXT("ProgressTrackingComponent"));
	
	// Progress Widget Component
	ProgressWidgetComponent = CreateDefaultSubobject<UInteractableWidgetComponent>(TEXT("ProgressBarWidgetComponent"));
	ProgressWidgetComponent->SetupAttachment(RootComponent);
	ProgressWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -600.0f));
	ProgressWidgetComponent->SetDrawSize(FIntPoint(50, 20));
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(ProgressTracker) && IsValid(ProgressWidgetComponent))
	{
		ProgressTracker->OnCompletion.BindDynamic(this, &ADoor::HandleProgressComplete);
		ProgressTracker->SetProgressWidgetReference(ProgressWidgetComponent->GetWidget());
	}
}

void ADoor::HandleProgressComplete()
{
	AFrogGameState* GameState = GetWorld()->GetGameState<AFrogGameState>();
	if (!IsValid(GameState)) return;
	
	OnProgressComplete.ExecuteIfBound(FacingDirection, UnlockPrice);
}

void ADoor::StartInteract()
{
	IInteractableInterface::StartInteract();
	ProgressTracker->StartProgress();
}

void ADoor::StopInteract()
{
	IInteractableInterface::StopInteract();
	ProgressTracker->StopProgress();
}

FVector ADoor::GetInteractableLocation() const
{
	return GetRootComponent()->GetComponentLocation();
}

void ADoor::SetHitboxActiveState(const bool bIsHitboxActive)
{
	if (!IsValid(InteractHitbox)) return;

	if (bIsHitboxActive)
	{
		InteractHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		InteractHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

