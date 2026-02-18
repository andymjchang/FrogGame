// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "Components/BoxComponent.h"
#include "FrogGameplay/ProgressTrackingComponent.h"
#include "GameState/FrogGameState.h"
#include "GameUI/Interactables/InteractableWidgetComponent.h" 

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Root Component
	USceneComponent* DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	RootComponent = DefaultRoot;

	// Hitbox
	Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	Hitbox->SetupAttachment(RootComponent);
	Hitbox->SetBoxExtent(FVector(64.f, 256.f, 128.f));
	Hitbox->SetRelativeLocation(FVector(0.0f, 0.0f, 128.0f));
	Hitbox->SetCollisionProfileName(TEXT("InteractListen"));
	
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
	
	if (IsValid(Hitbox))
	{
		Hitbox->OnComponentBeginOverlap.AddDynamic(this, &ADoor::HandleHitboxOverlapBegin);
		Hitbox->OnComponentEndOverlap.AddDynamic(this, &ADoor::HandleHitboxEndOverlap);
	}
	
	if (IsValid(ProgressTracker) && IsValid(ProgressWidgetComponent))
	{
		ProgressTracker->OnCompletion.BindDynamic(this, &ADoor::HandleProgressComplete);
		ProgressTracker->SetProgressWidget(ProgressWidgetComponent->GetWidget());
	}
}

void ADoor::ServerAttemptPurchase()
{
	if (!HasAuthority()) return;
	
	AFrogGameState* GameState = GetWorld()->GetGameState<AFrogGameState>();
	if (!IsValid(GameState)) return;
	
	if (BuyPrice <= GameState->GetMoney())
	{
		OnProgressComplete.ExecuteIfBound(FacingDirection);
	}
}

void ADoor::HandleProgressComplete()
{
	AFrogGameState* GameState = GetWorld()->GetGameState<AFrogGameState>();
	if (!IsValid(GameState)) return;
	
	ServerAttemptPurchase();
}

void ADoor::HandleHitboxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ProgressTracker->StartProgress();
}

void ADoor::HandleHitboxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	ProgressTracker->StopProgress();
}

void ADoor::SetActive(const bool bIsTrue)
{
	if (!IsValid(Hitbox)) return;

	if (bIsTrue)
	{
		Hitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		Hitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

