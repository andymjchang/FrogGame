// Fill out your copyright notice in the Description page of Project Settings.

#include "Station.h"
#include "InteractableData.h"
#include "GameState/FrogGameState.h"
#include "TimerManager.h"
#include "Components/PrimitiveComponent.h"
#include "Components/WidgetComponent.h"
#include "FrogCharacter/FrogCharacter.h"
#include "GameUI/Interactables/InteractableWidgetComponent.h"
#include "GameUI/Interactables/StationProgressBar.h"

AStation::AStation()
{
    PrimaryActorTick.bCanEverTick = true;

    // Inventory Widget
    ProgressBarWidgetComponent = CreateDefaultSubobject<UInteractableWidgetComponent>(TEXT("UProgressBarWidgetComponent"));
    ProgressBarWidgetComponent->SetupAttachment(RootComponent);
    ProgressBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -600.0f));
    ProgressBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    ProgressBarWidgetComponent->SetDrawSize(FIntPoint(100, 20));
}

void AStation::BeginPlay()
{
    Super::BeginPlay();
    
    OnAddedToInventory.AddUObject(this, &AStation::HandleInteractableAdded);
    
    if (ProgressBarWidgetComponent)
    {
        ProgressBarWidgetComponent->SetVisibility(false);
        ProgressBarWidget = Cast<UStationProgressBar>(ProgressBarWidgetComponent->GetWidget());
    }
}

void AStation::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    OnAddedToInventory.RemoveAll(this);

    Super::EndPlay(EndPlayReason);
}

void AStation::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsProcessing)
    {
        double CurrentTime = GetWorld()->GetTimeSeconds();

        if (ProgressBarWidgetComponent)
        {
            float Percentage = 0.0f;
            if (ProcessEndTime > ProcessStartTime)
            {
                Percentage = FMath::Clamp((float)((CurrentTime - ProcessStartTime) / (ProcessEndTime - ProcessStartTime)), 0.0f, 1.0f);
            }
            if (ProgressBarWidget.IsValid())
            {
                ProgressBarWidget->SetProgressPercent(Percentage);
            }
        }

        if (CurrentTime >= ProcessEndTime)
        {
            bIsProcessing = false;
            OnProcessingComplete();
        }
    }
}

void AStation::HandleInteractableAdded(AInteractable* InteractableToAdd)
{
    // Don't autostart if the added item is the finished item
    if (OfferedInteractable == this)
    {
        bIsProcessing = true;
        ProcessStartTime = GetWorld()->GetTimeSeconds();
        ProcessEndTime = ProcessStartTime + ProcessingDuration;

        if (ProgressBarWidgetComponent)
        {
            ProgressBarWidgetComponent->SetVisibility(true);
        }
    }
}

FGameplayTagContainer AStation::GatherAllTags() const
{
    FGameplayTagContainer AllTags;
    
    if (IsValid(Data))
    {
        AllTags.AppendTags(Data->GetOwnedTags());
    }
    
    for (AInteractable* Item : Inventory)
    {
        if (IsValid(Item) && IsValid(Item->GetData()))
        {
            AllTags.AppendTags(Item->GetData()->GetOwnedTags());
        }
    }
    
    return AllTags;
}

void AStation::OnProcessingComplete()
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Station: Processing complete!"));
    }
    
    AFrogGameState* GameState = GetWorld()->GetGameState<AFrogGameState>();
    if (!IsValid(GameState))
    {
        return;
    }
    
    if (ProgressBarWidgetComponent)
    {
        ProgressBarWidgetComponent->SetVisibility(false);
    }
    
    FGameplayTagContainer AllTags = GatherAllTags();
    
    TSubclassOf<AInteractable> ResultClass = GameState->GetResultInteractableClass(AllTags);
    
    if (ResultClass)
    {
        for (AInteractable* Item : Inventory)
        {
            if (IsValid(Item))
            {
                Item->Destroy();
            }
        }
        Inventory.Empty();
        
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        
        AInteractable* SpawnedResult = GetWorld()->SpawnActor<AInteractable>(
            ResultClass,
            GetActorLocation() + FVector(0, 0, 100), 
            FRotator::ZeroRotator,
            SpawnParams
        );
        
        if (IsValid(SpawnedResult))
        {
            OfferedInteractable = SpawnedResult;
            TryAddToInventory(SpawnedResult);
        }
    }
}