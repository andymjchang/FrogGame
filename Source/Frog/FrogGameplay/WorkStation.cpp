// Fill out your copyright notice in the Description page of Project Settings.

#include "WorkStation.h"

#include "ProgressTrackingComponent.h"
#include "Components/BoxComponent.h"

AWorkStation::AWorkStation()
{
    WorkHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WorkHitBox"));
    WorkHitBox->SetupAttachment(RootComponent);
    WorkHitBox->SetCollisionProfileName(TEXT("WorkListen"));
    WorkHitBox->InitBoxExtent(FVector(128.f, 128.f, 128.f));
    WorkHitBox->SetRelativeLocation(FVector(0.0f, 0.0f, 128.0f));
}

void AWorkStation::BeginPlay()
{
    Super::BeginPlay();
    
    if (IsValid(WorkHitBox))
    {
        WorkHitBox->OnComponentBeginOverlap.AddDynamic(this, &AWorkStation::OnWorkHitBoxBeginOverlap);
        WorkHitBox->OnComponentEndOverlap.AddDynamic(this, &AWorkStation::OnWorkHitBoxEndOverlap);
    }
}

void AWorkStation::HandleInteractableAdded(AInteractable* InteractableToAdd)
{
    UE_LOG(LogTemp, Log, TEXT("[%f] WorkStation: Item added, manual interaction required."), GetWorld()->GetTimeSeconds());
}

void AWorkStation::OnWorkHitBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Log, TEXT("HITBOX HIT FOR WORK"));
    ProgressTracker->StartProgress();
}

void AWorkStation::OnWorkHitBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    UE_LOG(LogTemp, Log, TEXT("HITBOX HIT FOR WORK STOPPED"));
    ProgressTracker->StopProgress();
}