// Fill out your copyright notice in the Description page of Project Settings.

#include "Station.h"
#include "InteractableData.h"
#include "GameState/IngredientMap.h"
#include "GameState/FrogGameState.h"
#include "TimerManager.h"
#include "Components/PrimitiveComponent.h"
#include "Components/WidgetComponent.h"
#include "FrogCharacter/FrogCharacter.h"
#include "GameUI/Interactables/InteractableWidgetComponent.h"

AStation::AStation()
{
	// Inventory Widget
	ProgressBarWidgetComponent = CreateDefaultSubobject<UInteractableWidgetComponent>(TEXT("UBillboardWidgetComponent"));
	ProgressBarWidgetComponent->SetupAttachment(RootComponent);
	ProgressBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -600.0f));
	ProgressBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	ProgressBarWidgetComponent->SetDrawSize(FIntPoint(100, 20));
}

void AStation::BeginPlay()
{
	Super::BeginPlay();
	ProgressBarWidgetComponent->SetVisibility(false);
}

bool AStation::TryAddToInventory(AInteractable* InteractableToAdd)
{
    // Call parent implementation
    bool bSuccess = Super::TryAddToInventory(InteractableToAdd);
    
    if (bSuccess)
    {
        // Start the processing timer
        GetWorldTimerManager().SetTimer(
            ProcessingTimerHandle,
            this,
            &AStation::OnProcessingComplete,
            ProcessingDuration,
            false  // Don't loop
        );
        
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, 
                FString::Printf(TEXT("Station: Started processing timer for %.1f seconds"), ProcessingDuration));
        }
    }
    
    return bSuccess;
}

FGameplayTagContainer AStation::GatherAllTags() const
{
    FGameplayTagContainer AllTags;
    
    // Add station's own tags
    if (IsValid(Data))
    {
        AllTags.AppendTags(Data->GetOwnedTags());
    }
    
    // Add tags from all items in inventory
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
    
    // Get the game state
    AFrogGameState* GameState = GetWorld()->GetGameState<AFrogGameState>();
    if (!IsValid(GameState))
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Station: Could not get FrogGameState"));
        }
        return;
    }
    
    // Gather all tags (station + inventory items)
    FGameplayTagContainer AllTags = GatherAllTags();
    
    // Log the tags we're looking up
    FString TagString;
    for (const FGameplayTag& Tag : AllTags)
    {
        TagString += Tag.ToString() + TEXT(", ");
    }
    
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, 
            FString::Printf(TEXT("Station: Looking up tags: %s"), *TagString));
    }
    
    // Look up the result class in the ingredient map
    TSubclassOf<AInteractable> ResultClass = GameState->GetResultInteractableClass(AllTags);
    
    if (ResultClass)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, 
                FString::Printf(TEXT("Station: Found result class! Spawning...")));
        }
        
        // Clear inventory and destroy items
        for (AInteractable* Item : Inventory)
        {
            if (IsValid(Item))
            {
                Item->Destroy();
            }
        }
        Inventory.Empty();
        
        // Spawn the result interactable
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        
        AInteractable* SpawnedResult = GetWorld()->SpawnActor<AInteractable>(
            ResultClass,
            GetActorLocation() + FVector(0, 0, 100), // Spawn above station
            FRotator::ZeroRotator,
            SpawnParams
        );
        
        if (IsValid(SpawnedResult))
        {
            // Try to add the result to this station
            bool bAdded = TryAddToInventory(SpawnedResult);
            
            if (bAdded)
            {
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, 
                        TEXT("Station: Result created and added to station!"));
                }
            }
            else
            {
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, 
                        TEXT("Station: Result created but couldn't be added to station"));
                }
            }
        }
        else
        {
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, 
                    TEXT("Station: Failed to spawn result"));
            }
        }
    }
    else
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, 
                TEXT("Station: No matching recipe found for these tags"));
        }
    }
}