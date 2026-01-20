// Fill out your copyright notice in the Description page of Project Settings.

#include "WorkStation.h"
#include "Components/BoxComponent.h"
#include "GameUI/Interactables/InteractableWidgetComponent.h"

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
    
    if (WorkHitBox)
    {
        WorkHitBox->OnComponentBeginOverlap.AddDynamic(this, &AWorkStation::OnWorkHitBoxBeginOverlap);
        WorkHitBox->OnComponentEndOverlap.AddDynamic(this, &AWorkStation::OnWorkHitBoxEndOverlap);
    }
}

void AWorkStation::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsProcessing && !bIsBeingWorked)
    {
        ProcessStartTime += DeltaTime;
        ProcessEndTime += DeltaTime;
    }
}

void AWorkStation::HandleInteractableAdded(AInteractable* InteractableToAdd)
{
    UE_LOG(LogTemp, Log, TEXT("[%f] WorkStation: Item added, manual interaction required."), GetWorld()->GetTimeSeconds());
}

void AWorkStation::StartWorking()
{
    if (!bIsProcessing && Inventory.Num() > 0)
    {
        bIsProcessing = true;
        ProcessStartTime = GetWorld()->GetTimeSeconds();
        ProcessEndTime = ProcessStartTime + ProcessingDuration;

        if (ProgressBarWidgetComponent)
        {
            ProgressBarWidgetComponent->SetVisibility(true);
        }

        UE_LOG(LogTemp, Log, TEXT("[%f] WorkStation: Started working."), GetWorld()->GetTimeSeconds());
    }

    bIsBeingWorked = true;
}

void AWorkStation::StopWorking()
{
    bIsBeingWorked = false;
    UE_LOG(LogTemp, Log, TEXT("[%f] WorkStation: Stopped working."), GetWorld()->GetTimeSeconds());
}

void AWorkStation::OnWorkHitBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Log, TEXT("HITBOX HIT FOR WORK"));
    StartWorking();
}

void AWorkStation::OnWorkHitBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    UE_LOG(LogTemp, Log, TEXT("HITBOX HIT FOR WORK STOPPED"));
    StopWorking();
}