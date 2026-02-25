// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "Components/BoxComponent.h"
#include "FrogGameplay/ProgressTrackingComponent.h"
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
		ProgressTracker->SetProgressWidget(ProgressWidgetComponent->GetWidget());
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
	ProgressTracker->StartProgress();
}

void ADoor::StopInteract()
{
	ProgressTracker->StopProgress();
}

void ADoor::StartHighlight()
{
}

void ADoor::StopHighlight()
{
}

FVector ADoor::GetInteractableLocation()
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

