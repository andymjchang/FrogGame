// Fill out your copyright notice in the Description page of Project Settings.

#include "Station.h"
#include "InteractableData.h"
#include "ProgressTrackingComponent.h"
#include "GameState/FrogGameState.h"
#include "TimerManager.h"
#include "Components/PrimitiveComponent.h"
#include "Components/WidgetComponent.h"
#include "FrogCharacter/FrogCharacter.h"
#include "GameUI/Interactables/InteractableWidgetComponent.h"
#include "GameUI/Interactables/StationProgressBar.h"

AStation::AStation()
{
    PrimaryActorTick.bCanEverTick = false;

    // Progress Tracking Component
    ProgressTrackingComponent = CreateDefaultSubobject<UProgressTrackingComponent>(TEXT("ProgressTrackingComponent"));
    
    // Progress Bar Widget
    ProgressBarWidgetComponent = CreateDefaultSubobject<UInteractableWidgetComponent>(TEXT("ProgressBarWidgetComponent"));
    ProgressBarWidgetComponent->SetupAttachment(RootComponent);
    ProgressBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -600.0f));
    ProgressBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    ProgressBarWidgetComponent->SetDrawSize(FIntPoint(100, 20));
}

void AStation::HandleInteractableAdded(AInteractable* InteractableToAdd)
{
}

void AStation::BeginPlay()
{
    Super::BeginPlay();
    
    OnAddedToInventory.AddUObject(this, &AStation::HandleInteractableAdded);
    
    if (IsValid(ProgressBarWidgetComponent))
    {
        ProgressTrackingComponent->OnCompletion.AddUObject(this, &AStation::OnProcessingComplete);
        
        if (UStationProgressBar* ProgressBarWidget = Cast<UStationProgressBar>(ProgressBarWidgetComponent->GetWidget()))
        {
            if (IsValid(ProgressTrackingComponent)) ProgressTrackingComponent->SetProgressWidget(ProgressBarWidget);
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
    UE_LOG(LogTemp, Log, TEXT("[%f] Station: Processing complete!"), GetWorld()->GetTimeSeconds());
    
    AFrogGameState* GameState = GetWorld()->GetGameState<AFrogGameState>();
    if (!IsValid(GameState))
    {
        return;
    }
    
    FGameplayTagContainer AllTags = GatherAllTags();
    
    TSubclassOf<AInteractable> ResultClass = GameState->GetRecipeResultClass(AllTags);
    
    if (ResultClass)
    {
        for (AInteractable* Item : Inventory)
        {
            if (IsValid(Item))
            {
                Item->Destroy();
            }
        }
        ClearInventory();
        
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
    else
    {
        OfferedInteractable = this;
        ClearInventory();
    }
}