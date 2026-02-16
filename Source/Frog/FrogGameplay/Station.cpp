// Fill out your copyright notice in the Description page of Project Settings.

#include "Station.h"

#include "ContainerComponent.h"
#include "InteractableData.h"
#include "ProgressTrackingComponent.h"
#include "GameState/FrogGameState.h"
#include "TimerManager.h"
#include "Components/PrimitiveComponent.h"
#include "Components/WidgetComponent.h"
#include "GameUI/Interactables/InteractableWidgetComponent.h"

AStation::AStation()
{
    PrimaryActorTick.bCanEverTick = false;

    // Progress Tracking Component
    ProgressTracker = CreateDefaultSubobject<UProgressTrackingComponent>(TEXT("ProgressTrackingComponent"));
    
    // Progress Bar Widget Component
    ProgressWidgetComponent = CreateDefaultSubobject<UInteractableWidgetComponent>(TEXT("ProgressBarWidgetComponent"));
    ProgressWidgetComponent->SetupAttachment(RootComponent);
    ProgressWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -600.0f));
    ProgressWidgetComponent->SetDrawSize(FIntPoint(100, 20));
}

void AStation::HandleInteractableAdded(AInteractable* InteractableToAdd)
{
}

void AStation::BeginPlay()
{
    Super::BeginPlay();
    
    if (IsValid(ProgressWidgetComponent) && IsValid(ProgressTracker))
    {
        ProgressTracker->OnCompletion.BindDynamic(this, &AStation::HandleProcessingComplete);
        ProgressTracker->SetProgressWidget(ProgressWidgetComponent->GetWidget());
    }
}

FGameplayTagContainer AStation::GatherAllTags() const
{
    FGameplayTagContainer AllTags;
    
    if (IsValid(Data))
    {
        AllTags.AppendTags(Data->GetOwnedTags());
    }
    
    for (AInteractable* Item : ContainerComponent->GetInventory())
    {
        if (IsValid(Item) && IsValid(Item->GetData()))
        {
            AllTags.AppendTags(Item->GetData()->GetOwnedTags());
        }
    }
    
    return AllTags;
}

void AStation::HandleProcessingComplete()
{
    UE_LOG(LogTemp, Log, TEXT("[%f] Station: Processing complete!"), GetWorld()->GetTimeSeconds());

    const AFrogGameState* GameState = GetWorld()->GetGameState<AFrogGameState>();
    if (!IsValid(GameState))
    {
        return;
    }

    const FGameplayTagContainer AllTags = GatherAllTags();

    if (const TSubclassOf<AInteractable> ResultClass = GameState->GetRecipeResultClass(AllTags))
    {
        for (AInteractable* Item : ContainerComponent->GetInventory())
        {
            if (IsValid(Item))
            {
                Item->Destroy();
            }
        }
        ContainerComponent->ClearInventory();
        
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
            ContainerComponent->TryAddToInventory(SpawnedResult);
        }
    }
    else
    {
        OfferedInteractable = this;
        ContainerComponent->ClearInventory();
    }
}