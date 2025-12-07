// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableComponent.h"

// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // Create and configure the interact hitbox
    // InteractHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractHitBox"));
    // InteractHitBox->SetupAttachment(this);
}

// Called when the game starts
void UInteractableComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // Bind overlap events
    OnComponentBeginOverlap.AddDynamic(this, &UInteractableComponent::OnOverlapBegin);
    OnComponentEndOverlap.AddDynamic(this, &UInteractableComponent::OnOverlapEnd);
    SetCollisionProfileName(TEXT("ItemHitBox"));
}

void UInteractableComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor)
    {
        StartInteract.Broadcast(OtherActor);
    }
}

void UInteractableComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor)
    {
        StopInteract.Broadcast(OtherActor);
    }
}

// Called every frame
void UInteractableComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}