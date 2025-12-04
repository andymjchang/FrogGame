// Fill out your copyright notice in the Description page of Project Settings.

#include "Station.h"
#include "Components/PrimitiveComponent.h"

AStation::AStation()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AStation::BeginPlay()
{
	Super::BeginPlay();
    
	InteractionCollision = Cast<UPrimitiveComponent>(GetDefaultSubobjectByName(TEXT("StationCollisionComponent")));
    
	if (InteractionCollision)
	{
		InteractionCollision->OnComponentBeginOverlap.AddDynamic(this, &AStation::OnOverlapBegin);
		InteractionCollision->OnComponentEndOverlap.AddDynamic(this, &AStation::OnOverlapEnd);
	}
}

void AStation::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							  bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("OVERLAP START%d"), Capacity));
		On_Interact();
	}
}

void AStation::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
					  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("OVERLAP END%d"), Capacity));
	GetWorldTimerManager().ClearTimer(InteractionTimerHandle);
}

void AStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStation::On_Interact()
{
	//Should check player for inventory, if player has items in inventory, should either add or swap items, if not, should see if inventory is at capacity, if yes, should start timer
	GetWorldTimerManager().SetTimer(InteractionTimerHandle, this, &AStation::CompleteInteraction, 2.0f, false);
}

void AStation::CompleteInteraction()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("INTERACTION COMPLETE %d"), Capacity));
}